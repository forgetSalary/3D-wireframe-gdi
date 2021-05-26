#include "3D.h"
#include "vector"

uint8_t char_to_digit[256];

struct InternStr {
    const char* str;
    size_t len;
};

arena_t str_pool = EMPTY_ARENA;
std::vector<InternStr> interns;

const char* str_intern_range(const char* start, const char* end){
    size_t len = end - start;

    for (size_t i = 0; i < interns.size(); ++i) {
        if (interns[i].len = len && strncmp(interns[i].str, start, len) == 0){
            return interns[i].str;
        }
    }
    char* str = (char*)arena_alloc(&str_pool,len+1);
    memcpy(str,start,len);
    str[len] = 0;
    interns.push_back({str,len});
    return str;
}

const char *str_intern(const char* str){
    return str_intern_range(str,str+strlen(str));
}

enum TokenKind {
    TOKEN_FLOAT = 128,
    TOKEN_INT,
    TOKEN_NAME,
    TOKEN_COMMENT,
    TOKEN_VERTEX,
    TOKEN_FACE,
    TOKEN_C4D_OBJ,
    // ...
};

struct Token {
    TokenKind kind;
    const char *start;
    const char *end;
    union {
        double float_val;
        uint32_t int_val;
        const char* name;
    };
};

Token token;
const char* stream;

void fatal(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("FATAL: ");
    vprintf(fmt, args);
    va_end(args);
    exit(1);
}

void scan_int() {
    uint32_t val = 0;
    for (;;) {
        uint32_t digit = char_to_digit[*stream];
        if (digit == 0 && *stream != '0') {
            break;
        }
        if (val > (UINT64_MAX - digit) / 10) {
            fatal("Integer literal overflow");
            while (isdigit(*stream)) {
                stream++;
            }
            val = 0;
            break;
        }
        val = val*10 + digit;
        stream++;
    }
    token.kind = TOKEN_INT;
    token.int_val = val;
}

void scan_float() {
    const char* start = stream;
    while (isdigit(*stream)) {
        stream++;
    }
    if (*stream == '.') {
        stream++;
    }
    while (isdigit(*stream)) {
        stream++;
    }
    
    double val = strtod(start, NULL);
    if (val == HUGE_VAL || val == -HUGE_VAL) {
        fatal("Expected digit after float literal exponent, found '%c'.", *stream);
    }
    token.kind = TOKEN_FLOAT;
    token.float_val = val;
}

void next_token() {
    switch (*stream) {
        case '-':
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
            bool negative;
            if (*stream == '-'){
                negative = true;
                stream++;
            }
            else negative = false;

            const char* num_start = stream;
            while (isdigit(*stream)) {
                stream++;
            }
            char c = *stream;
            stream = num_start;
            if (c == '.') {
                scan_float();
                token.float_val = token.float_val;
            } else {
                scan_int();
                if (negative){
                    if(token.kind == TOKEN_FLOAT){
                        token.float_val = -token.float_val;
                    }else if(token.kind == TOKEN_INT){
                        token.int_val = ~(token.int_val -1);
                    }
                }
            }

            break;
        }
    case 'a': case 'b': case 'c': case 'd':case 'e': case 'f':case 'g': case 'h':case 'i':case 'j':
    case 'k': case 'l': case 'm': case 'n':case 'o': case 'p':case 'q': case 'r':case 's':case 't':
    case 'u': case 'v': case 'w': case 'x':case 'y': case 'z':
    case 'A': case 'B': case 'C': case 'D':case 'E': case 'F':case 'G': case 'H':case 'I':case 'J':
    case 'K': case 'L': case 'M': case 'N':case 'O': case 'P':case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X':case 'Y': case 'Z':
    case '_': {
        token.start = stream;
        while (isalnum(*stream) || *stream == '_') {
            stream++;
        }
        token.kind = TOKEN_NAME;
        token.name = str_intern_range(token.start,stream);

        if ((stream - token.start) == 1){
            if(token.name[0] == 'v'){
                token.kind = TOKEN_VERTEX;
            }else if(token.name[0] == 'f'){
                token.kind = TOKEN_FACE;
            }else if(token.name[0] == 'o'){
                while (*stream != '\n') {
                    stream++;
                }
                token.kind = TOKEN_C4D_OBJ;
            }
        }

        break;
    }
    case '#':{
        token.kind = TOKEN_COMMENT;
        while (*stream != '\n') {
            stream++;
        }
        break;
    }
    default:
        token.kind = (TokenKind)(*stream++);
        break;
    }
    token.end = stream;
}

void init_stream(const char* str) {
    stream = str;
    char_to_digit['0'] = 0;
    char_to_digit['1'] = 1;
    char_to_digit['2'] = 2;
    char_to_digit['3'] = 3;
    char_to_digit['4'] = 4;
    char_to_digit['5'] = 5;
    char_to_digit['6'] = 6;
    char_to_digit['7'] = 7;
    char_to_digit['8'] = 8;
    char_to_digit['9'] = 9;
    next_token();
}

void init_stream_from_file(FILE* from){
    size_t grow_size = 1024;
    char* buffer = (char*)malloc(grow_size);
    int len = 0,grow_len = 0;

    int end = 0;
    for(;end!=EOF;){
        end = fscanf(from,"%c",buffer+len);
        len++;
        grow_len++;
        if (grow_len == grow_size){
            buffer = (char*)realloc(buffer,len+grow_size);
            grow_len = 1;
        }
    }
    buffer[len-1] = '\0';
    init_stream(buffer);
}

inline bool is_token(TokenKind kind){
    return token.kind == kind;
}

inline bool is_token_name(const char* name){
    return TOKEN_NAME && token.name == name;
}

inline bool match_token(TokenKind kind){
    if(is_token(kind)){
        next_token();
        return true;
    } else{
        return false;
    }
}

inline bool expect_token(TokenKind kind){
    if(is_token(kind)){
        next_token();
        return true;
    } else{
        fatal("unexpected token");
        return false;
    }
}

POINT3 vertex;
Edge edge;
std::vector<uint32_t> indexes;

bool parse_vertex(){
    next_token();
    expect_token(TokenKind(' '));
    if(is_token(TOKEN_FLOAT)){
        vertex.x = token.float_val*100;
    }else if(is_token(TOKEN_INT)){
        vertex.x = token.int_val*100;
    }else{
        fatal("unexpected token");
    }

    next_token();
    expect_token(TokenKind(' '));
    if(is_token(TOKEN_FLOAT)){
        vertex.y = token.float_val*100;
    }else if(is_token(TOKEN_INT)){
        vertex.y = token.int_val*100;
    }else{
        fatal("unexpected token");
    }

    next_token();
    expect_token(TokenKind(' '));
    if(is_token(TOKEN_FLOAT)){
        vertex.z = token.float_val*100;
    }else if(is_token(TOKEN_INT)){
        vertex.z = token.int_val*100;
    }else{
        fatal("unexpected token");
    }

    next_token();
    expect_token(TokenKind('\n'));
    if (is_token(TOKEN_VERTEX)){
        return false;
    }else{
        return true;
    }
}

bool parse_edge(){
    next_token();
    int count = 0;
    while(token.kind != '\n'){
        expect_token(TokenKind(' '));
        if (is_token(TOKEN_INT)){
            indexes.push_back(token.int_val-1);
            count++;
        }else{
            fatal("unexpected token");
        }
        next_token();
    }
    edge.vertices_count = count;

    next_token();
    if (is_token(TOKEN_FACE)){
        return false;
    }else{
        return true;
    }
}

Object* parse_obj(FILE* file){
    init_stream_from_file(file);

    std::vector<POINT3> vertexes;
    std::vector<Edge> edges;

    bool last = false;
    while(!is_token(TokenKind('\0'))) {
        switch (token.kind) {
            case TOKEN_VERTEX:
                while(!last){
                    last = parse_vertex();
                    vertexes.push_back(vertex);
                }
                last = false;
                break;
            case TOKEN_FACE:
                while(!last){
                    last = parse_edge();
                    edges.push_back(edge);
                }
                last = false;
                break;
            default:
                next_token();
                break;
        }
    }
    Object* obj = new Object;
    obj->set_heap(HeapCreate(HEAP_GENERATE_EXCEPTIONS,0,0));

    POINT3* WPT = (POINT3*)HeapAlloc(obj->heap(),HEAP_GENERATE_EXCEPTIONS,vertexes.size()*sizeof(POINT3));
    POINT* SPT = (POINT*)HeapAlloc(obj->heap(),HEAP_GENERATE_EXCEPTIONS,vertexes.size()*sizeof(POINT));
    Edge* _edges = (Edge*)HeapAlloc(obj->heap(),HEAP_GENERATE_EXCEPTIONS,edges.size()*sizeof(Edge));
    memcpy(WPT,vertexes.data(),vertexes.size()*sizeof(POINT3));
    memcpy(_edges,edges.data(),edges.size()*sizeof(Edge));

    int offset = 0;
    for (int i = 0; i < edges.size(); ++i) {
        _edges[i].order = (uint32_t*)HeapAlloc(obj->heap(),HEAP_GENERATE_EXCEPTIONS,sizeof(uint32_t)*_edges[i].vertices_count);
        for (int j = 0; j < _edges[i].vertices_count; ++j) {
            _edges[i].order[j] = indexes[j+offset];
        }
        offset+=_edges[i].vertices_count;
        _edges[i].vertices_count--;
    }

    *obj = Object(obj->heap(),WPT,SPT,_edges,vertexes.size(),edges.size());
    return obj;
}