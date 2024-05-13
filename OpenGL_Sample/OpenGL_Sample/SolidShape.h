#pragma once

// 図形の描画
#include "Shape.h"

//=== 三角形による描画 ===//
class SolidShape : public Shape
{
public:

	//--- コンストラクタ ---//
	// size :        頂点位置の次元
	// vertexCount : 頂点の数
	// vertex :      頂点属性を格納した配列
	// indexCount :  頂点インデックスの要素数
	// index :       原点のインデックスを格納した配列
	SolidShape(GLint size, GLsizei vertexCount, const Object::Vertex* vertex)
		:Shape(size, vertexCount, vertex)
	{

	}

	//--- 描画の実行 ---//
	virtual void execute() const
	{
		// 三角形で描画する
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
};
