#pragma once

#pragma comment(lib, "C:/Script/Ruby/lib/libmsvcrt-ruby220.lib")


typedef unsigned long VALUE;
typedef unsigned long ID;

#define SIGNED_VALUE long


enum ruby_special_consts {
#if USE_FLONUM
	RUBY_Qfalse = 0x00,		/* ...0000 0000 */
	RUBY_Qtrue = 0x14,		/* ...0001 0100 */
	RUBY_Qnil = 0x08,		/* ...0000 1000 */
	RUBY_Qundef = 0x34,		/* ...0011 0100 */

	RUBY_IMMEDIATE_MASK = 0x07,
	RUBY_FIXNUM_FLAG = 0x01,	/* ...xxxx xxx1 */
	RUBY_FLONUM_MASK = 0x03,
	RUBY_FLONUM_FLAG = 0x02,	/* ...xxxx xx10 */
	RUBY_SYMBOL_FLAG = 0x0c,	/* ...0000 1100 */
#else
	RUBY_Qfalse = 0,		/* ...0000 0000 */
	RUBY_Qtrue = 2,		/* ...0000 0010 */
	RUBY_Qnil = 4,		/* ...0000 0100 */
	RUBY_Qundef = 6,		/* ...0000 0110 */

	RUBY_IMMEDIATE_MASK = 0x03,
	RUBY_FIXNUM_FLAG = 0x01,	/* ...xxxx xxx1 */
	RUBY_FLONUM_MASK = 0x00,	/* any values ANDed with FLONUM_MASK cannot be FLONUM_FLAG */
	RUBY_FLONUM_FLAG = 0x02,
	RUBY_SYMBOL_FLAG = 0x0e,	/* ...0000 1110 */
#endif
	RUBY_SPECIAL_SHIFT = 8
};


enum ruby_value_type {
	RUBY_T_NONE = 0x00,

	RUBY_T_OBJECT = 0x01,
	RUBY_T_CLASS = 0x02,
	RUBY_T_MODULE = 0x03,
	RUBY_T_FLOAT = 0x04,
	RUBY_T_STRING = 0x05,
	RUBY_T_REGEXP = 0x06,
	RUBY_T_ARRAY = 0x07,
	RUBY_T_HASH = 0x08,
	RUBY_T_STRUCT = 0x09,
	RUBY_T_BIGNUM = 0x0a,
	RUBY_T_FILE = 0x0b,
	RUBY_T_DATA = 0x0c,
	RUBY_T_MATCH = 0x0d,
	RUBY_T_COMPLEX = 0x0e,
	RUBY_T_RATIONAL = 0x0f,

	RUBY_T_NIL = 0x11,
	RUBY_T_TRUE = 0x12,
	RUBY_T_FALSE = 0x13,
	RUBY_T_SYMBOL = 0x14,
	RUBY_T_FIXNUM = 0x15,

	RUBY_T_UNDEF = 0x1b,
	RUBY_T_NODE = 0x1c,
	RUBY_T_ICLASS = 0x1d,
	RUBY_T_ZOMBIE = 0x1e,

	RUBY_T_MASK = 0x1f
};


#define Qfalse ((VALUE)RUBY_Qfalse)
#define Qtrue  ((VALUE)RUBY_Qtrue)
#define Qnil   ((VALUE)RUBY_Qnil)
#define ANYARGS ...

#define T_MASK   RUBY_T_MASK

struct RBasic {
	VALUE flags;
	const VALUE klass;
};
#define BUILTIN_TYPE(x) (int)(((struct RBasic*)(x))->flags & T_MASK)


#define R_CAST(st)   (struct st*)
#define RBASIC(obj)  (R_CAST(RBasic)(obj))
#define RARRAY(obj)  (R_CAST(RArray)(obj))



#define FL_USHIFT    12

#define FL_USER0     (((VALUE)1)<<(FL_USHIFT+0))
#define FL_USER1     (((VALUE)1)<<(FL_USHIFT+1))
#define FL_USER2     (((VALUE)1)<<(FL_USHIFT+2))
#define FL_USER3     (((VALUE)1)<<(FL_USHIFT+3))
#define FL_USER4     (((VALUE)1)<<(FL_USHIFT+4))

#define RARRAY_EMBED_FLAG FL_USER1
/* FL_USER2 is for ELTS_SHARED */
#define RARRAY_EMBED_LEN_MASK (FL_USER4|FL_USER3)
#define RARRAY_EMBED_LEN_SHIFT (FL_USHIFT+3)
#define RARRAY_EMBED_LEN_MAX 3

#define RARRAY_LEN(a) \
    ((RBASIC(a)->flags & RARRAY_EMBED_FLAG) ? \
     (long)((RBASIC(a)->flags >> RARRAY_EMBED_LEN_SHIFT) & \
	 (RARRAY_EMBED_LEN_MASK >> RARRAY_EMBED_LEN_SHIFT)) : \
     RARRAY(a)->as.heap.len)

struct RArray {
	struct RBasic basic;
	union {
		struct {
			long len;
			union {
				long capa;
				VALUE shared;
			} aux;
			const VALUE *ptr;
		} heap;
		const VALUE ary[RARRAY_EMBED_LEN_MAX];
	} as;
};


extern "C" {
	void	ruby_sysinit(int *, char ***);
	void	ruby_init_stack(volatile VALUE*);
	void	ruby_init(void);
	void	ruby_init_loadpath(void);
	int		ruby_cleanup(volatile int);
	void	ruby_finalize(void);
	void	rb_load(VALUE, int);
	void	rb_load_protect(VALUE, int, int*);

	VALUE	rb_str_new_cstr(const char*);

	void	ruby_stop(int);

	VALUE	rb_gv_set(const char*, VALUE);
	VALUE	rb_gv_get(const char*);

	char *	rb_string_value_ptr(volatile VALUE*);
	long	rb_num2int(VALUE);

	ID		rb_intern(const char*);
	VALUE	rb_funcall(VALUE, ID, int, ...);

	VALUE	rb_ary_shift(VALUE);
	VALUE	rb_ary_entry(VALUE, long);
	VALUE	rb_ary_push(VALUE, VALUE);

	void	rb_define_global_function(const char*, VALUE(*)(ANYARGS), int);


	char *	rb_string_value_ptr(volatile VALUE*);
	char *	rb_string_value_cstr(volatile VALUE*);
	long	rb_num2int(VALUE);
	VALUE	rb_int2inum(SIGNED_VALUE);
}




extern "C" {
	BOOL InitRuby();
}