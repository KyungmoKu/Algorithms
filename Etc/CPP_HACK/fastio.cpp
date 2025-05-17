/**
 * FASTIO
 * Just use as if we use cin, cout. + u dont have to get rid of cin.tie(0)->sync_with_stdio(0);
 * call cout.setprecision(prec) for double precision. default is 6
 * __int128_t -> input or print is ok
 * you can call numeric_limits min/max // most implementations can't input or output numeric_limits<int>::min();
 * cin.geti() , cin.geti<ll>(), cin.getline() all works.
 * Special function: vector, pair input/output => vector<int> v(3); cin>>v; // Works!
 *
 * TEST
    static const __uint128_t UINT128_MAX =__uint128_t(__int128_t(-1L));
    static const __int128_t INT128_MAX = UINT128_MAX >> 1;
    static const __int128_t INT128_MIN = -INT128_MAX - 1;
    __int128_t x;
    cin >> x; // works
    cout<< x << '\n'; // works
    cout<< INT128_MIN << '\n'; // works
**/
#define FASTIO 1
#if FASTIO
constexpr int ISZ = 1 << 19, OSZ = 1 << 19;
class _IN{
    private:
        char buf[ISZ+1], *p=buf;
        int __END_FLAG__{}, __GETLINE_FLAG__{};
    public:
        explicit operator bool(){return !__END_FLAG__;}
        inline bool isEnd(char c){ return c == '\0';}
        inline bool isBlank(char c){return c=='\n'||c==' '||c=='\t'||c=='\r';}
        inline char read(){
            if(__END_FLAG__) return '\0';
            if(isEnd(*p)) {
                buf[fread(buf,sizeof(char),ISZ,stdin)] = 0;
                p = buf;
                if(isEnd(*p)) return '\0';
            };
            return *p++;
        }
        inline void scan(char&c){
            do c=read(); while(isBlank(c));
            if(isEnd(c)) __END_FLAG__ = true;
        }
        inline void scan(string&s){
            s.clear(); char c; scan(c);
            while(!isBlank(c) && !isEnd(c)) s.push_back(c), c=read();
        }
        inline void scan(float&f){string t; scan(t); f=stof(t);}
        inline void scan(double&f){string t; scan(t); f=stod(t);}
        inline void scan(long double&f){string t; scan(t); f=stold(t);}
        template<typename T=int> inline T geti(){
            char c; scan(c);
            T res=0; bool sign = true;
            if(c=='-') sign = false, c=read();
            else if(c == '+') c = read();
            while('0'<=c && c<='9'){
                res = (res << 3) + (res << 1);
                c -= '0'; res += sign ? c : -c;
                c = read();
            }
            return res;
        }
        inline string getline(){
            string s; char c = read();
            for(;c!='\n' && !isEnd(c); c = read()) s.push_back(c);
            if(__GETLINE_FLAG__) __END_FLAG__ = true;
            if(isEnd(c)) __GETLINE_FLAG__ =  true;
            return s;
        }
        template<typename T1,typename T2> inline void scan(pair<T1,T2>&p){scan(p.first);scan(p.second);}
        template<typename T> inline void scan(T&n){ n = geti<T>(); }
        template<typename T> inline void scan(vector<T>&v){for(auto&k:v) scan(k);}
        template<typename T, typename... Args> inline void scan(T&n, Args&...args){
            scan(n); scan(args...);
        }
        _IN* tie(int _){return this;}
        void sync_with_stdio(int _){}
        void exceptions(bool _){}
} _in;
class _OUT{
    private:
    char buf[OSZ+1],*p=buf, tmp[42];
    int prec = 6;
    public:
        explicit operator bool(){return true;}
        inline void flush(){fwrite(buf,1,p-buf,stdout); p=buf;}
        inline void print(const char c){{if(p==buf+OSZ) flush();} *p++=c; }
        inline void print(const char*s){for(int i=0;s[i];i++) print(s[i]); }
        inline void print(const string&s){for(char c:s) print(c);}
        // floating point precision
        inline void setprecision(int precision){prec=precision;}
        inline void print(const long double f){
            std::stringstream stream;
            stream << std::fixed << std::setprecision(prec) << f;
            print(stream.str());
        }
        inline void print(const double f){print((long double)f);}
        inline void print(const float f){print((long double)f);}
        template<typename T> inline void print(const vector<T>&v){for(auto k:v) print(k), print(' ');}
        template<typename T1,typename T2> inline void print(const pair<T1,T2>&p){print(p.first); print(' '); print(p.second);}
        // we assume T is integer
        template<typename T> inline void print(T ans){
            bool is_minimum = (ans<0) && ((ans<<1) == 0); // check whether this is the minimum of data type
            if(ans<0){
                print('-');
                if(is_minimum) ans = ~ans;
                else ans *= -1;
            }
            int cnt=0;
            do tmp[cnt++]=(ans%10)+'0', ans/=10; while(ans>0);
            if(is_minimum) ++tmp[0]; // we assume this is not 9, in-fact, c++ integer type {min}'s last digit is always '8'
            for(;cnt;) print(tmp[--cnt]);
        }
        ~_OUT(){flush();}
} _out;
template<typename T> _IN& operator>> (_IN&in, T&i){in.scan(i); return in; }
template<typename T> _OUT& operator<< (_OUT&out, T i){out.print(i); return out; }
#define cin _in
#define cout _out

#else
template<typename T1,typename T2> istream& operator>> (istream&in, pair<T1,T2>&p){ in>>p.first>>p.second; return in; }
template<typename T> istream& operator>> (istream&in, vector<T>&v){for(auto&k:v) in>>k; return in; }
template<typename T1,typename T2> ostream& operator<< (ostream&out, pair<T1,T2>&p){out<<p.first<<' '<<p.second; return out; }
template<typename T> ostream& operator<< (ostream&out, vector<T> v){for(auto k:v) out<<k<<' '; return out; }

#endif
// END FASTIO