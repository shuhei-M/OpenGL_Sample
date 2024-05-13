#pragma once

#include <array>
#include <GL/glew.h>

//=== 図形データ ===//
class Object
{
private:

	// 頂点配列オブジェクト名
	GLuint vao;

	// 頂点バッファ・オブジェクト名
	GLuint vbo;

	// インデックスの頂点バッファオブジェクト
	GLuint ibo;

public:

	//--- 頂点属性 ---//
	struct Vertex
	{
		// 位置
		GLfloat position[3];

		// 法線
		GLfloat normal[3];
	};

	//--- コンストラクタ ---//
	// size :        頂点位置の次元
	// vertexCount : 頂点の数
	// vertex :      頂点属性を格納した配列
	// indexCount :  頂点インデックスの要素数
	// index :       原点のインデックスを格納した配列
	Object(GLint size, GLsizei vertexCount, const Vertex* vertex,
		GLsizei indexCount, const GLuint* index)
	{
		// 頂点配列オブジェクト
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// 頂点バッファオブジェクト
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER,
			vertexCount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

		// 結合されている頂点バッファオブジェクトを in 変数から参照できるようにする
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			static_cast<char*>(0) + sizeof vertex->position);
		glEnableVertexAttribArray(1);

		// インデックスの頂点バッファオブジェクト
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indexCount * sizeof(GLuint), index, GL_STATIC_DRAW);
	}

	//--- デストラクタ ---//
	virtual ~Object()
	{
		// 頂点配列オブジェクトを削除する
		glDeleteBuffers(1, &vao);

		// 頂点バッファ・オブジェクトを削除する
		glDeleteBuffers(1, &vbo);

		// インデックスの頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &ibo);
	}

private:

	//--- コピーコンストラクタによるコピー禁止 ---//
	Object(const Object& o);

	//--- 代入によるコピー禁止 ---//
	Object& operator=(const Object& o);

public:

	//--- 頂点配列オブジェクトの結合 ---//
	void bind() const
	{
		// 描画する頂点配列オブジェクトを指定する
		glBindVertexArray(vao);
	}
};
