#pragma once

#include <memory>

// 図形データ
#include "Object.h"

//=== 図形の描画 ===//
class Shape
{
private:

	// 図形データ
	std::shared_ptr<const Object> object;

protected:

	// 描画に使う頂点の数
	const GLsizei vertexCount;

public:

	//--- コンストラクタ ---//
	// size :        頂点位置の次元
	// vertexCount : 頂点の数
	// vertex :      頂点属性を格納した配列
	// indexCount :  頂点インデックスの要素数
	// index :       原点のインデックスを格納した配列
	Shape(GLint size, GLsizei vertexCount, const Object::Vertex* vertex,
		GLsizei indexCount = 0, const GLuint* index = NULL)
		: object(new Object(size, vertexCount, vertex, indexCount, index))
		, vertexCount(vertexCount)
	{

	}

	//--- 描画 ---//
	void draw() const
	{
		// 頂点オブジェクトを結合する
		object->bind();

		// 描画を実行する
		execute();
	}

	//--- 描画の実行 ---//
	virtual void execute() const
	{
		// 折れ線で描画する
		glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
	}
};
