#include <cstdio>
#include <cstring>

template <const size_t _Size = 16>
class quick_io {
private:
	char buf_in[1 << _Size], buf_out[1 << _Size];
	size_t index_in, index_out;
	FILE *in, *out;

public:
	inline void flush_in() {
		fread(buf_in, sizeof(char), 1 << _Size, in), index_in = 0;
	}
	inline void flush_out() {
		fwrite(buf_out, index_out, 1, out), index_out = 0;
	}
	inline quick_io& operator>>(char& ch) {
		if (index_in == (1 << _Size)) flush_in();
		ch = buf_in[index_in++];
		return *this;
	}
	inline quick_io& operator>>(char* str) {
		size_t origin = strlen(str);
		char ch = 0;
		size_t len = 0;
		do {
			*this >> ch;
			if (!isgraph(ch)) break;
			str[len++] = ch;
		} while (true);
		if (len < origin)
			while (len < origin) str[len++] = 0;
		return *this;
	}
	template <typename T>
	inline quick_io& operator>>(T& value) {
		value = 0;
		short sign = 1;
		char ch = 0;
		do sign = ch == '-' ? -1 : sign, *this >> ch;
		while (ch < '0' || ch > '9');
		do value = (value << 3) + (value << 1) + ch - '0', *this >> ch;
		while ('0' <= ch && ch <= '9');
		value *= sign;
		return *this;
	}
	inline quick_io& operator<<(const char ch) {
		if (index_out == (1 << _Size)) flush_out();
		buf_out[index_out++] = ch;
		return *this;
	}
	inline quick_io& operator<<(const char* str) {
		size_t len = strlen(str);
		for (size_t i = 0; i < len; ++i) *this << str[i];
		return *this;
	}
	inline quick_io& operator<<(char* str) {
		size_t len = strlen(str);
		for (size_t i = 0; i < len; ++i) *this << str[i];
		return *this;
	}
	template <typename T>
	inline quick_io& operator<<(T value) {
		if (value < 0) *this << '-', value = -value;
		static size_t stack[50], dep = 0;
		do stack[++dep] = value % 10, value /= 10;
		while (value);
		do *this << (char)(stack[dep--] + '0');
		while (dep);
		return *this;
	}
	quick_io(FILE* in = stdin, FILE* out = stdout)
		: index_in(0), index_out(0), in(in), out(out) {}
	~quick_io() { flush_out(); }
};
