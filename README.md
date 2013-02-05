glew4kb
=======

Automate OpenGL extension initialization for 4k intro.

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

