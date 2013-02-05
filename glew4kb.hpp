/*! <-Press % key to go to English manual. (If you are using vim)
	There is english manual after japanese manual.

	・概要
	glew4kbは4k introのようなプログラムサイズを小さくしたいプログラムでも
	OpenGLの拡張関数を簡単に使えるようにする為のライブラリです.

	・必要なもの
	The OpenGL Extension Wrangler Library (GLEW) (http://glew.sourceforge.net/)

	・動作環境
	OpenGLが使えるC++コンパイラ.
	の予定だが, 現在の所はVC++2008でのみ動作確認.

	・導入方法
	このライブラリはヘッダファイル1枚のみです.
	インクルードパスが通った所に置くとかして下さい.
	GLEWがインストールされていなければインストールして下さい.

	・使用方法
	デバッグ用とリリース用の2つのビルド構成が必要です.
	・デバッグビルド
		- 標準ライブラリ使用.
		- GLEWで拡張関数を初期化.
		- GL/glew.hの後でglew4kb.hppをインクルード.
	・リリースビルド
		- 標準ライブラリをリンクしない.
		- GLEWは使用しない.
		- glew4kb.hppをインクルードしない.
		- glew4kb_exts.hをインクルード.
	デバッグビルドを実行し, プログラム内で使用されている全ての拡張関数が呼ばれるまで走らせて下さい.
	このときに呼ばれた拡張関数がglew4kb内で記録されます.
	プログラム終了時にglew4kb内の静的なオブジェクトのデストラクタで
	glew4kb_exts.hというファイルがカレントディレクトリに出力されます.
	(デストラクタが呼ばれない場合はglew4kb::output()関数を呼んで下さい)
	このファイルにはプログラム内で使用さた拡張関数のみを初期化するコードが書かれています.

	・オプション
	オプションはこのヘッダファイルをインクルードする前に#defineして下さい.
		- GLEW4KB_LIST
			これを#defineしないとglew4kbは何もしない.
		- GLEW4KB_METHOD_ID 
			ソースコードを生成する関数を選択する.
			glew4kbには5種類のソースコードを生成する関数が実装されている.
			プログラムの内容, 使用する拡張関数の数等によって,
			どの関数が最も小さいコードを生成できるかが違ってくる.
			#define GLEW4KB_METHOD_ID 3
			のように0～4の数字を指定する.
			デフォルトは0
		- GLEW4KB_EXCLUDE_LIST
			除外する拡張関数名を指定する.
			glGetShaderInfoLogのようなデバッグ時に使用したいが
			リリースビルドでは使わない拡張関数があったとき
			glew4kbが生成するソースコードでそれらの拡張関数を除外する事によって
			より小さいコードを得られる.
			使用例 & デフォルト値:
#	define GLEW4KB_EXCLUDE_LIST	\
		"glIsShader", "glGetShaderiv", "glGetShaderInfoLog",	\
		"glIsProgram", "glGetProgramiv", "glGetProgramInfoLog",	\
		"glCheckFramebufferStatus"


	・サンプルコード
#ifdef NDEBUG
//4k introビルド用のコード.
#	include <GL/gl.h>
#	include <GL/glext.h>
#	define GLEW4KB_INLINE inline
#	include "glew4kb_exts.h"
#else
//デバッグビルド用のコード.
#	include <GL/glew.h>
#	include <GL/gl.h>
#	define GLEW4KB_LIST
#	include <glew4kb.hpp>
#endif

void main()
{
	//拡張関数を初期化する前にウィンドウとOpenGLを初期化.
#ifdef NDEBUG
	glew4kb_setup_GL_ext();
#else
	GLenum glewError;
	if((glewError = glewInit()) != GLEW_OK)
	{
		cerr << "Error: " << glewGetErrorString(glewError) << endl;
		return;
	}
#endif

	//普通に拡張関数が使える.
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, (const GLchar**)&source, NULL);
	glCompileShader(shader);

	...
}

	・問題点
		- VC++2008以外のコンパイラ, 環境でテストしていない.
			Linux用コードがあるけどテストしてない.
		- GLEWの実装詳細に依存している.
			未来のGLEWでも動作するかはわからない.
		- GLEWの複数コンテクスト機能(GLEW MX)が使えない.
			メガデモで複数コンテクストを使う人はおらんやろ.

	・仕組み
	GLEWのglew.hを読むと各拡張関数がGLEW_GET_FUNというマクロで置き換えられているのがわかります.
	glew4kbではGLEW_GET_FUNマクロを置き換えて, 拡張関数が呼ばれる度にその関数名を記録します.
	そしてプログラム終了時に記録を元に4k intro向けのコードが作成されます.
	(´・д・｀)(＾ω＾;)
*/

/*!
	- What is glew4kb?
		Automate OpenGL extension initialization for 4k intro.

	- Dependency
		The OpenGL Extension Wrangler Library (GLEW) (http://glew.sourceforge.net/)

	- Platform
		C++ compiler which can use OpenGL and be used for 4k intros.
		But currently glew4kb is only tested with VC++2008.

	- How to install
		glew4kb is a header only library.
		Please put it somewhere your compiler can find.
		And install GLEW if you didn't installed it.

	- How to use
		You need both debug build and release build(build for 4k intro).
		- Debug build
			- Use standard C/C++ library.
			- Initialize OpenGL extensions with GLEW.
			- Include glew4kb.hpp after GL/glew.h
		- Release build
			- Don't link standard C/C++ library.
			- Don't use GLEW.
			- Don't include glew4kb.hpp
			- Include glew4kb_exts.h

		Please execute debug build until all OpenGL extensions are called.
		Then, all OpenGL extensions are recorded in glew4kb.
		When your program is quitting, destractor of static object in glew4kb is called,
		and it output glew4kb_exts.h in current directory.
		(If you don't call destractor of static object, please call glew4kb::output())
		There is a code initialize OpenGL extensions for 4k intro in that file.

	- Options
		You can set options by #define following macros before #include <glew4kb.hpp>
		- GLEW4KB_LIST
			If you don't define this macro, glew4kb do nothing.
		- GLEW4KB_METHOD_ID
			Select a function to generate code.
			There are 5 functions to generate code in glew4kb.
			Most smallest one varies depending on number of OpenGL extensions and
			bit patterns of your program.
		- GLEW4KB_EXCLUDE_LIST
			Specify OpenGL extensions you want to exclude.
			If there are functions like glGetShaderInfoLog
			which you want to use in Debug build but not in Release build,
			set them in GLEW4KB_EXCLUDE_LIST.
			Example and default value:
#	define GLEW4KB_EXCLUDE_LIST	\
		"glIsShader", "glGetShaderiv", "glGetShaderInfoLog",	\
		"glIsProgram", "glGetProgramiv", "glGetProgramInfoLog",	\
		"glCheckFramebufferStatus"

	- Sample code
#ifdef NDEBUG
//Build for 4k intro.
#	include <GL/gl.h>
#	include <GL/glext.h>
#	define GLEW4KB_INLINE inline
#	include "glew4kb_exts.h"
#else
//Debug build
#	include <GL/glew.h>
#	include <GL/gl.h>
#	define GLEW4KB_LIST
#	include <glew4kb.hpp>
#endif

void main()
{
	//Initialize window and OpenGL before initializing OpenGL extensions.

#ifdef NDEBUG
	glew4kb_setup_GL_ext();
#else
	GLenum glewError;
	if((glewError = glewInit()) != GLEW_OK)
	{
		cerr << "Error: " << glewGetErrorString(glewError) << endl;
		return;
	}
#endif

	//You can use OpenGL extensions as usual.
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, (const GLchar**)&source, NULL);
	glCompileShader(shader);

	...
}

	- Known problems
		- This code is not tested with any compilers except VC++2008.
			There are code for Linux, but they are not tested.
		- glew4kb depends on implementation detail of the GLEW.
			It mignt not work in future GLEW.
		- Multiple Rendering Contexts (GLEW MX) of GLEW doesn't work.
			Who use Multiple Rendering Contexts in demoscene?

	- How it work
		Please read GL/glew.h. All OpenGL extensions are define with GLEW_GET_FUN macro.
		glew4kb redefines this macro so that everytime a extension is called,
		name of it is recorded.
		Finally, source code is generated from the list of used extensions.
*/

/*!
#define GLEW4KB_LIST
*/

#ifndef GLEW4KB_H
#define GLEW4KB_H

#include <algorithm>
#include <set>
#include <string>
#include <iterator>
#include <iostream>
#include <fstream>
#include <utility>
#include <cctype>


#ifdef GLEW_GET_FUN
#	undef GLEW_GET_FUN
#	define GLEW_GET_FUN(x) (glew4kb::get(x, #x, __LINE__, __FILE__).get())
#endif

#ifdef WGLEW_GET_FUN
#	undef WGLEW_GET_FUN
#	define WGLEW_GET_FUN(x) (glew4kb::get(x, #x, __LINE__, __FILE__).get())
#endif

#ifdef GLXEW_GET_FUN
#	undef GLXEW_GET_FUN
#	define GLXEW_GET_FUN(x) (glew4kb::get(x, #x, __LINE__, __FILE__).get())
#endif

#ifndef GLEW4KB_EXCLUDE_LIST
#	define GLEW4KB_EXCLUDE_LIST	\
		"glIsShader", "glGetShaderiv", "glGetShaderInfoLog",	\
		"glIsProgram", "glGetProgramiv", "glGetProgramInfoLog",	\
		"glCheckFramebufferStatus"
#endif

#ifndef GLEW4KB_METHOD_ID
#	define GLEW4KB_METHOD_ID 0
#endif

namespace glew4kb
{
template<typename T>
struct before_after_check
{
	before_after_check(T pfunc, const char* name, int line, const char* file):
		pfunc(pfunc), name(name), line(line), file(file)
	{
#ifdef GLEW4KB_LIST
		detail::list_of_exts::get_instance().regist(name);
#endif
	}

	~before_after_check()
	{
	}

	T get()
	{
		return pfunc;
	}

	T pfunc;
	const char* name;
	int line;
	const char* file;
};

template<typename T>
inline before_after_check<T> get(T pfunc, const char* name, int line, const char* file)
{
	return before_after_check<T>(pfunc, name, line, file);
}

namespace detail
{
//Return OpenGL function name from glew pointer name.
inline std::string glfnc_name_from_glew_name(const std::string& name)
{
	using namespace std;

	string glprefix, glewprefix;

	if(name.compare(0, string("__wglew").size(), "__wglew") == 0)
	{
		glprefix = "wgl";
		glewprefix = "__wglew";
	}else if(name.compare(0, string("__glewX").size(), "__glewX") == 0)
	{
		glprefix = "glX";
		glewprefix = "__glewX";
	}else if(name.compare(0, string("__glew").size(), "__glew") == 0)
	{
		glprefix = "gl";
		glewprefix = "__glew";
	}

	return glprefix + name.substr(glewprefix.size());
}

//Return (pointer to OpenGL function) typename from OpenGL function name.
inline std::string gl_typename_from_glfnc_name(const std::string& name)
{
	std::string type_name = name;
	std::transform(type_name.begin(), type_name.end(), type_name.begin(), &toupper);
	return std::string("PFN") + type_name + "PROC";
}


template<typename InputItr>
inline std::ostream& output_glextnames_array(
	std::ostream& os, InputItr first, InputItr last)
{
	os << "\nstatic char* glext_names = {\n";
	for(InputItr i=first; i!=last; ++i)
	{
		const std::string& func_name = *i;
		os <<
			"\"" <<
			func_name << "\\0\"\n";
	}
	os << "\"\\0\"\n};\n\n";

	return os;
}

inline std::ostream& output_common_header(std::ostream& os)
{
	os << "#pragma once\n\n";

	os <<
		"#ifdef _WIN32													\n"
		"#	define	GLEW4KB_GETPROCADDRESS(n)							\\\n"
		"				wglGetProcAddress(n)							\n"
		"#elif defined(__linux__) 										\n"
		"#	define 	GLEW4KB_GETPROCADDRESS(n)							\\\n"
		"				glXGetProcAddress((const unsigned char *)n)		\n"
		"#else 															\n"
		"#	error Unknow platform 										\n"
		"#endif 														\n"
		"																\n"
		;

	return os;
}

inline std::ostream& output_common_setup_func_header(std::ostream& os)
{
	os <<
	"\n\n"
	"//#define GLEW4KB_INLINE inline(or __forceinline) before including this file!\n"
	"GLEW4KB_INLINE void glew4kb_setup_GL_ext()\n"
	"{\n";

	return os;
}

/*!
	ダミーの初期化関数を出力する.
	Output dummy initializing function.
*/
inline std::ostream& output_common_dummy_setup_func(std::ostream& os)
{
	output_common_setup_func_header(os) <<
	"\t//This is a dummy setup function so that you dont need to write #if #endif block.\n"

	"\t//This function must be removed by compiler or linker(crinkler).\n"
	"}\n\n";

	return os;
}

struct list_of_exts
{
	void regist(const char* name)
	{
		exts.insert(std::string(name));
	}

	static list_of_exts& get_instance()
	{
		//list_of_extsはシングルトンで, プログラム終了時にデストラクタが呼ばれる.
		//list_of_exts is a singleton class and its destractor is called when exiting this program.
		static list_of_exts instance;

		return instance;
	}

	void output()
	{
		if(exts.empty())
		{
			return;
		}

		{
			string_set tmp_exts;
			std::transform
			(
				exts.begin(), exts.end(),
				std::inserter(tmp_exts, tmp_exts.begin()),
				&glfnc_name_from_glew_name
			);
			exts.swap(tmp_exts);
		}

		const char* exc_list[] = {GLEW4KB_EXCLUDE_LIST};
		for(int i=0; i<sizeof(exc_list)/sizeof(exc_list[0]); ++i)
		{
			string_set_itr itr = exts.find(exc_list[i]);
			if(itr!=exts.end())
			{
				exts.erase(itr);
			}
		}

		std::ofstream ofs
		(
			"glew4kb_exts.h",
			std::ios_base::out | std::ios_base::trunc
		);

		output(ofs);
		exts.clear();
	}

private:

	inline void output(std::ostream& os);

	list_of_exts(){}

	~list_of_exts()
	{
		this->output();
	}

	typedef std::set<std::string>	string_set;
	typedef string_set::iterator	string_set_itr;
	string_set exts;
};
}

/*!
	4k introでOpenGL拡張関数を初期化するためのヘッダファイルを出力する.
	Output header file which initialize OpenGL extension functions for 4k intro.
*/
inline void output()
{
	glew4kb::detail::list_of_exts::get_instance().output();
}

namespace detail
{
#if GLEW4KB_METHOD_ID == 0
inline void list_of_exts::output(std::ostream& os)
{
	//w23 told me this code. Thank you!
	//This code is compressed very well.

	output_common_header(os);

	int c = 0;
	for(string_set_itr i=exts.begin(); i!=exts.end(); ++i, ++c)
	{
		const std::string& func_name = *i;
		const std::string func_type_name = gl_typename_from_glfnc_name(func_name);
		os <<
			"#define " << func_name << "\t" <<
			"((" <<
			func_type_name << ")" <<
			"glew4kb_glext_fns[" << c << "])" <<
			"\n";
	}

	output_glextnames_array(os, exts.begin(), exts.end());

	os <<
	"#define GLEXT_FN_COUNT 64 /*too many, but who cares*/ //(sizeof(_glext_names)/sizeof(_glext_names[0]))\n"
	"void* glew4kb_glext_fns[GLEXT_FN_COUNT];						\n";

	output_common_setup_func_header(os) <<
	"	const char* pn = glext_names;								\n"
	"	void** pp = glew4kb_glext_fns;								\n"
	"	for(;;)														\n"
	"	{															\n"
	"		if (*pn == 0) break;									\n"
	"		*pp++ = GLEW4KB_GETPROCADDRESS(pn);						\n"
	"																\n"
	"		// win32												\n"
	"		while(*(pn++) != 0);									\n"
	"#if DEBUG														\n"
	"		static int i = 0;										\n"
	"		if (glewkb_glext_fns[i] == 0)							\n"
	"		{														\n"
	"			printf												\n"
	"			(													\n"
	"				\"failed to load \\\"%s\\\": %08x\\n\",			\n"
	"				glew4kb_glext_names[i], GetLastError()			\n"
	"			);													\n"
	"			fflush(stdout);										\n"
	"			abort();											\n"
	"		}														\n"
	"		++i;													\n"
	"#endif															\n"
	"	}															\n"
	"}																\n";
}
#elif GLEW4KB_METHOD_ID == 1
inline void list_of_exts::output(std::ostream& os)
{
	output_common_dummy_setup_func
	(
		output_common_header(os)
	);

	for(string_set_itr i=exts.begin(); i!=exts.end(); ++i)
	{
		const std::string& func_name = *i;
		const std::string func_type_name = gl_typename_from_glfnc_name(func_name);
		os
			<< "#define "
			<< func_name << "\t"
			<< "((" << func_type_name << ")GLEW4KB_GETPROCADDRESS(\""
			<< func_name << "\"))\n";
	}
}
#elif GLEW4KB_METHOD_ID == 2
inline void list_of_exts::output(std::ostream& os)
{
	/*
		Reference:
		http://www.iquilezles.org/www/material/isystem1k4k/isystem1k4k.htm
	*/

	output_common_header(os);

	os <<
	"#define GLEXT_FN_COUNT " << exts.size() << "\n" <<
	"static void* glew4kb_glext_fns[GLEXT_FN_COUNT];					\n"
	"																	\n"
	"static char* glext_names[] = {										\n";
	for(string_set_itr i=exts.begin(); i!=exts.end(); ++i)
	{
		const std::string& func_name = *i;
		os <<
			"\"" << func_name << "\"";
		string_set_itr j = i;
		++j;
		os << ((j!=exts.end()) ? ",\n" : "\n");
	}
	os << "};\n\n";

	size_t index = 0;
	for(string_set_itr i=exts.begin(); i!=exts.end(); ++i, ++index)
	{
		const std::string& func_name = *i;
		const std::string func_type_name = gl_typename_from_glfnc_name(func_name);
		os
			<< "#define "
			<< func_name << "\t"
			<< "((" << func_type_name << ")glew4kb_glext_fns[" << index << "])\n";
	}

#if 1
	output_common_setup_func_header(os) <<
	"	for( int i=0; i<GLEXT_FN_COUNT; i++ )								\n"
	"	{																	\n"
	"		glew4kb_glext_fns[i] = GLEW4KB_GETPROCADDRESS(glext_names[i]);	\n"
	"	}																	\n"
	"}																		\n";
#elif 0
	output_common_setup_func_header(os) <<
	"	int i=GLEXT_FN_COUNT;												\n"
	"	do																	\n"
	"	{																	\n"
	"		--i;															\n"
	"		glew4kb_glext_fns[i] = GLEW4KB_GETPROCADDRESS(glext_names[i]);	\n"
	"	}while(i);															\n"
	"}																		\n";
#else
	output_common_setup_func_header(os) <<
	"	int i=0;\n\n";
	for(string_set_itr i=exts.begin(); i!=exts.end(); ++i)
	{
		os <<
		"	glew4kb_glext_fns[i] = GLEW4KB_GETPROCADDRESS(glext_names[i]);	\n"
		"	++i;															\n";
	}
	os << '}';
#endif
}
#elif GLEW4KB_METHOD_ID == 3
inline void list_of_exts::output(std::ostream& os)
{
	output_common_header(os);

	for(string_set_itr i=exts.begin(); i!=exts.end(); ++i)
	{
		const std::string& func_name = *i;
		const std::string func_type_name = gl_typename_from_glfnc_name(func_name);
		os << func_type_name << ("\t") << func_name << (";\n");
	}

	output_common_setup_func_header(os);

	for(string_set_itr i=exts.begin(); i!=exts.end(); ++i)
	{
		const std::string& func_name = *i;
		const std::string func_type_name = gl_typename_from_glfnc_name(func_name);
		os
			<< "\t"
			<< func_name << "\t=("
			<< func_type_name << ")GLEW4KB_GETPROCADDRESS(\""
			<< func_name << "\");\n";
	}

	os << ("}\n");
}
#elif GLEW4KB_METHOD_ID == 4
inline void list_of_exts::output(std::ostream& os)
{
	output_common_dummy_setup_func
	(
		output_common_header(os)
	);

	output_glextnames_array(os, exts.begin(), exts.end());

	size_t offset = 0;
	for(string_set_itr i=exts.begin(); i!=exts.end(); ++i)
	{
		const std::string& func_name = *i;
		const std::string func_type_name = gl_typename_from_glfnc_name(func_name);
		os
			<< "#define "
			<< func_name << "\t"
			<< "((" << func_type_name << ")GLEW4KB_GETPROCADDRESS("
			<< "glext_names+" << offset << "))\n";
		offset += func_name.size()+1;
	}
}
#else
#error "おかしいよ! You are wrong!! Your GLEW4KB_METHOD_ID is too large or below 0"
#endif

}

}


#endif
