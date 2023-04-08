#ifndef CS564_PROJECT_SQLITE3_HPP
#define CS564_PROJECT_SQLITE3_HPP

#include "sqlite3.h"

#include <array>
#include <cstdarg>
#include <exception>
#include <sstream>
#include <string>
#include <utility>

namespace sqlite {

class Exception : public std::exception {
public:
  explicit Exception(int code, std::string message)
      : code_(code), message_(std::move(message)) {}

  [[nodiscard]] const char *what() const noexcept override {
    return message_.c_str();
  }

  [[nodiscard]] int code() const { return code_; }

private:
  int code_;
  std::string message_;
};

class Result {
public:
  Result() : code_(SQLITE_OK) {}

  explicit Result(int code) : code_(code) {}

  operator int() { return code_; } // NOLINT (allow implicit conversion)

  void expect(int code) const {
    if (code_ != code) {
      std::ostringstream stream;
      stream << sqlite3_errstr(code_) << " (" << code_ << ")";
      throw Exception(code_, stream.str());
    }
  }

private:
  int code_;
};

enum TransactionBehavior { DEFERRED, IMMEDIATE, EXCLUSIVE };

struct Null {};

template <typename Data, typename Size> struct GenericBlob {
  GenericBlob(const Data *data, Size size, void (*destructor)(void *) = nullptr)
      : data(data), size(size), destructor(destructor) {}

  const Data *data;
  Size size;
  void (*destructor)(void *);
};

struct Blob : GenericBlob<void, int> {
  Blob(const void *data, int size, void (*destructor)(void *) = nullptr)
      : GenericBlob(data, size, destructor) {}
};

struct Blob64 : GenericBlob<void, sqlite3_uint64> {
  Blob64(const void *data, sqlite3_uint64 size,
         void (*destructor)(void *) = nullptr)
      : GenericBlob(data, size, destructor) {}
};

struct Text : GenericBlob<char, int> {
  Text(const char *data, int size, void (*destructor)(void *) = nullptr)
      : GenericBlob(data, size, destructor) {}
};

struct Text16 : GenericBlob<void, int> {
  Text16(const void *data, int size, void (*destructor)(void *) = nullptr)
      : GenericBlob(data, size, destructor) {}
};

struct Text64 : GenericBlob<char, sqlite3_uint64> {
  Text64(const char *data, sqlite3_uint64 size,
         void (*destructor)(void *) = nullptr,
         unsigned char encoding = SQLITE_UTF8)
      : GenericBlob(data, size, destructor), encoding(encoding) {}

  unsigned char encoding;
};

struct ZeroBlob {
  explicit ZeroBlob(int size) : size(size) {}

  int size;
};

struct ZeroBlob64 {
  explicit ZeroBlob64(sqlite3_uint64 size) : size(size) {}

  sqlite3_uint64 size;
};

struct Pointer {
  explicit Pointer(void *pointer, char *type,
                   void (*destructor)(void *) = nullptr)
      : pointer(pointer), type(type), destructor(destructor) {}

  void *pointer;
  char *type;
  void (*destructor)(void *);
};

class Statement {
public:
  Statement() = default;

  Statement(sqlite3_stmt *p_stmt, std::shared_ptr<sqlite3> p_conn)
      : p_stmt_(p_stmt, sqlite3_finalize), p_conn_(std::move(p_conn)) {}

  [[nodiscard]] const std::shared_ptr<sqlite3_stmt> &ptr() { return p_stmt_; }

  Result bind_blob(int i, const void *data, int size,
                   void (*destructor)(void *) = nullptr) {
    return Result(sqlite3_bind_blob(p_stmt_.get(), i, data, size, destructor));
  }

  Result bind_blob64(int i, const void *data, sqlite_uint64 size,
                     void (*destructor)(void *) = nullptr) {
    return Result(
        sqlite3_bind_blob64(p_stmt_.get(), i, data, size, destructor));
  }

  Result bind_double(int i, double v) {
    return Result(sqlite3_bind_double(p_stmt_.get(), i, v));
  }

  Result bind_int(int i, int v) {
    return Result(sqlite3_bind_int(p_stmt_.get(), i, v));
  }

  Result bind_int64(int i, sqlite3_int64 v) {
    return Result(sqlite3_bind_int64(p_stmt_.get(), i, v));
  }

  Result bind_null(int i) {
    return Result(sqlite3_bind_null(p_stmt_.get(), i));
  }

  Result bind_text(int i, const char *data, int size,
                   void (*destructor)(void *) = nullptr) {
    return Result(sqlite3_bind_text(p_stmt_.get(), i, data, size, destructor));
  }

  Result bind_text(int i, const char *v) {
    return bind_text(i, v, -1, SQLITE_TRANSIENT);
  }

  Result bind_text(int i, const std::string &v) {
    return bind_text(i, v.c_str());
  }

  Result bind_text16(int i, const void *data, int size,
                     void (*destructor)(void *) = nullptr) {
    return Result(
        sqlite3_bind_text16(p_stmt_.get(), i, data, size, destructor));
  }

  Result bind_text64(int i, const char *data, sqlite3_uint64 size,
                     void (*destructor)(void *) = nullptr,
                     unsigned char encoding = SQLITE_UTF8) {
    return Result(sqlite3_bind_text64(p_stmt_.get(), i, data, size, destructor,
                                      encoding));
  }

  Result bind_value(int i, const sqlite3_value *v) {
    return Result(sqlite3_bind_value(p_stmt_.get(), i, v));
  }

  Result bind_pointer(int i, void *pointer, const char *type,
                      void (*destructor)(void *) = nullptr) {
    return Result(
        sqlite3_bind_pointer(p_stmt_.get(), i, pointer, type, destructor));
  }

  Result bind_zeroblob(int i, int size) {
    return Result(sqlite3_bind_zeroblob(p_stmt_.get(), i, size));
  }

  Result bind_zeroblob64(int i, sqlite3_uint64 size) {
    return Result(sqlite3_bind_zeroblob64(p_stmt_.get(), i, size));
  }

  template <typename... T> Result bind_all(T... args) {
    return bind_all(std::make_integer_sequence<int, sizeof...(T)>(), args...);
  }

  Result step() { return Result(sqlite3_step(p_stmt_.get())); }

  const void *column_blob(int i) {
    return sqlite3_column_blob(p_stmt_.get(), i);
  }

  double column_double(int i) {
    return sqlite3_column_double(p_stmt_.get(), i);
  }

  int column_int(int i) { return Result(sqlite3_column_int(p_stmt_.get(), i)); }

  sqlite3_int64 column_int64(int i) {
    return sqlite3_column_int64(p_stmt_.get(), i);
  }

  const unsigned char *column_text(int i) {
    return sqlite3_column_text(p_stmt_.get(), i);
  }

  const void *column_text16(int i) {
    return sqlite3_column_text16(p_stmt_.get(), i);
  }

  sqlite3_value *column_value(int i) {
    return sqlite3_column_value(p_stmt_.get(), i);
  }

  int column_bytes(int i) { return sqlite3_column_bytes(p_stmt_.get(), i); }

  int column_bytes16(int i) { return sqlite3_column_bytes16(p_stmt_.get(), i); }

  int column_type(int i) { return sqlite3_column_type(p_stmt_.get(), i); }

  Result reset() { return Result(sqlite3_reset(p_stmt_.get())); }

  Result execute(size_t &count) {
    count = 0;
    while (step() == SQLITE_ROW) {
      ++count;
    }
    return reset();
  }

  Result execute() {
    size_t count;
    return execute(count);
  }

  template <typename T> Result bind(int i, T v) = delete;

private:
  template <int... i, typename... T>
  Result bind_all(std::integer_sequence<int, i...>, T... args) {
    int rc;
    (void)(((rc = bind(i + 1, args)) == SQLITE_OK) && ...);
    return Result(rc);
  }

  std::shared_ptr<sqlite3_stmt> p_stmt_;
  std::shared_ptr<sqlite3> p_conn_;
};

template <> Result Statement::bind<Blob>(int i, Blob v) {
  return bind_blob(i, v.data, v.size, v.destructor);
}

template <> Result Statement::bind<Blob64>(int i, Blob64 v) {
  return bind_blob64(i, v.data, v.size, v.destructor);
}

template <> Result Statement::bind<double>(int i, double v) {
  return bind_double(i, v);
}

template <> Result Statement::bind<int>(int i, int v) { return bind_int(i, v); }

template <> Result Statement::bind<sqlite3_int64>(int i, sqlite3_int64 v) {
  return bind_int64(i, v);
}

template <> Result Statement::bind<Null>(int i, Null) { return bind_null(i); }

template <> Result Statement::bind<Text>(int i, Text v) {
  return bind_text(i, v.data, v.size, v.destructor);
}

template <> Result Statement::bind<const char *>(int i, const char *v) {
  return bind_text(i, v);
}

template <>
Result Statement::bind<const std::string &>(int i, const std::string &v) {
  return bind_text(i, v);
}

template <> Result Statement::bind<Text16>(int i, Text16 v) {
  return bind_text16(i, v.data, v.size, v.destructor);
}

template <> Result Statement::bind<Text64>(int i, Text64 v) {
  return bind_text64(i, v.data, v.size, v.destructor, v.encoding);
}

template <> Result Statement::bind<sqlite3_value *>(int i, sqlite3_value *v) {
  return bind_value(i, v);
}

template <> Result Statement::bind<Pointer>(int i, Pointer v) {
  return bind_pointer(i, v.pointer, v.type, v.destructor);
}

template <> Result Statement::bind<ZeroBlob>(int i, ZeroBlob v) {
  return bind_zeroblob(i, v.size);
}

template <> Result Statement::bind<ZeroBlob64>(int i, ZeroBlob64 v) {
  return bind_zeroblob64(i, v.size);
}

class Connection {
public:
  Connection() = default;

  explicit Connection(sqlite3 *p_conn) : p_conn_(p_conn, sqlite3_close) {
    prepare(begin_[0], "BEGIN DEFERRED").expect(SQLITE_OK);
    prepare(begin_[0], "BEGIN IMMEDIATE").expect(SQLITE_OK);
    prepare(begin_[0], "BEGIN EXCLUSIVE").expect(SQLITE_OK);
    prepare(commit_, "COMMIT").expect(SQLITE_OK);
  }

  [[nodiscard]] const std::shared_ptr<sqlite3> &ptr() { return p_conn_; }

  Result prepare(Statement &stmt, const std::string &sql) {
    int rc;
    sqlite3_stmt *p_stmt;
    rc = sqlite3_prepare_v2(p_conn_.get(), sql.c_str(), -1, &p_stmt, nullptr);
    if (rc == SQLITE_OK) {
      stmt = Statement(p_stmt, p_conn_);
    }
    return Result(rc);
  }

  Result execute(const std::string &sql) {
    return Result(
        sqlite3_exec(p_conn_.get(), sql.c_str(), nullptr, nullptr, nullptr));
  }

  Result begin(TransactionBehavior behavior = DEFERRED) {
    return begin_[behavior].execute();
  }

  Result commit() { return commit_.execute(); }

  int changes() { return sqlite3_changes(p_conn_.get()); }

  const char *errmsg() { return sqlite3_errmsg(p_conn_.get()); }

private:
  std::shared_ptr<sqlite3> p_conn_;
  std::array<Statement, 3> begin_;
  Statement commit_;
};

class Database {
public:
  explicit Database(std::string filename) : filename_(std::move(filename)) {}

  Result connect(Connection &conn) {
    int rc;
    rc = sqlite3_initialize();
    if (rc == SQLITE_OK) {
      sqlite3 *p_conn;
      rc = sqlite3_open(filename_.c_str(), &p_conn);
      if (rc == SQLITE_OK) {
        conn = Connection(p_conn);
      }
    }
    return Result(rc);
  }

private:
  std::string filename_;
};

Result initialize() { return Result(sqlite3_initialize()); }

Result shutdown() { return Result(sqlite3_shutdown()); }

template <typename... Args> Result config(int option, Args... args) {
  return Result(sqlite3_config(option, args...));
}

} // namespace sqlite

#endif // CS564_PROJECT_SQLITE3_HPP
