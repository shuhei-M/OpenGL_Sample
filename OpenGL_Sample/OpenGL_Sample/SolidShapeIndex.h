#pragma once

// インデックスを使った図形の描画
#include "ShapeIndex.h"

//=== インデックスを使った三角形による描画 ===//
class SolidShapeIndex : public ShapeIndex
{
public:

	//--- コンストラクタ ---//
	// size :        頂点位置の次元
	// vertexCount : 頂点の数
	// vertex :      頂点属性を格納した配列
	// indexCount :  頂点インデックスの要素数
	// index :       原点のインデックスを格納した配列
	SolidShapeIndex(GLint size, GLsizei vertexCount, const Object::Vertex* vertex,
		GLsizei indexCount, const GLuint* index)
		:ShapeIndex(size, vertexCount, vertex, indexCount, index)
	{

	}

	//--- 描画の実行 ---//
	virtual void execute() const
	{
		// 線分群で描画する
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
};