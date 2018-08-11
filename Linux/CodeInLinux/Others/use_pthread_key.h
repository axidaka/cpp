//#include <malloc.h>

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>


void write_to_thread_log (const char* message);
void close_thread_log (void* thread_log);
void* thread_function (void* argv);
void use_pthread_key();


# if defined(WIN32) || defined(WIN64)
	template<typename T>
	struct TSS{
		typedef void (* Destructor)(void * ptr) ;
		TSS(Destructor destructor=0) : _destructor(destructor){
			_key = ::TlsAlloc() ;
			assert(_key!=TLS_OUT_OF_INDEXES) ;
		}
		~TSS(){	::TlsFree(_key) ;}
		inline T *		get(){ return static_cast<T *>(TlsGetValue(_key)) ;	}
		inline void		set(T * p){ TlsSetValue(_key,p) ; }
		inline void		finalize(){
			T * p = get() ;
			if( p ){
				if( _destructor ) _destructor(p) ;
				set(0) ;
			}
		}
	private:
		DWORD  				_key ;
		const Destructor	_destructor ;
	};
# else    //  with pthread
	template<typename T>
	class TSS{
	public:
		typedef void (* Destructor)(void * ptr) ;
		TSS(Destructor destructor=0){	pthread_key_create(&_key,(Destructor)destructor) ;	}
		~TSS(){	pthread_key_delete(_key) ;	}
		inline T *		get(){ return static_cast<T *>(pthread_getspecific(_key)) ;	}
		inline void	  set(T * p){ pthread_setspecific(_key,p) ; }
		inline void		finalize(){}	//	do nothing , pthread api support auto destructor callback when thread exit
	private:
		pthread_key_t  		_key ;
	};
# endif


void write_to_thread_log_class (const char* message);

void* thread_function_class (void* argv);

void use_pthread_key_class();


