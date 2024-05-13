#pragma once

// 図形の描画
#include "Shape.h"

//=== インデックスを使った図形の描画 ===//
class ShapeIndex : public Shape
{
protected:

	// 描画に使う頂点の数
	const GLsizei indexCount;

public:

	// コンストラクタ
	// size :        頂点位置の次元
	// vertexCount : 頂点の数
	// vertex :      頂点属性を格納した配列
	// indexCount :  頂点インデックスの要素数
	// index :       原点のインデックスを格納した配列
	ShapeIndex(GLint size, GLsizei vertexCount, const Object::Vertex* vertex,
		GLsizei indexCount, const GLuint* index)
		:Shape(size, vertexCount, vertex, indexCount, index)
		,indexCount(indexCount)
	{

	}

	virtual void execute() const
	{
		// 線分群で描画する
		glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
	}
};
