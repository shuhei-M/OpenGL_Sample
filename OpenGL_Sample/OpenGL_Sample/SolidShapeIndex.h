#pragma once

// �C���f�b�N�X���g�����}�`�̕`��
#include "ShapeIndex.h"

//=== �C���f�b�N�X���g�����O�p�`�ɂ��`�� ===//
class SolidShapeIndex : public ShapeIndex
{
public:

	//--- �R���X�g���N�^ ---//
	// size :        ���_�ʒu�̎���
	// vertexCount : ���_�̐�
	// vertex :      ���_�������i�[�����z��
	// indexCount :  ���_�C���f�b�N�X�̗v�f��
	// index :       ���_�̃C���f�b�N�X���i�[�����z��
	SolidShapeIndex(GLint size, GLsizei vertexCount, const Object::Vertex* vertex,
		GLsizei indexCount, const GLuint* index)
		:ShapeIndex(size, vertexCount, vertex, indexCount, index)
	{

	}

	//--- �`��̎��s ---//
	virtual void execute() const
	{
		// �����Q�ŕ`�悷��
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
};