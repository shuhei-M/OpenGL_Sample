#pragma once

// �}�`�̕`��
#include "Shape.h"

//=== �O�p�`�ɂ��`�� ===//
class SolidShape : public Shape
{
public:

	//--- �R���X�g���N�^ ---//
	// size :        ���_�ʒu�̎���
	// vertexCount : ���_�̐�
	// vertex :      ���_�������i�[�����z��
	// indexCount :  ���_�C���f�b�N�X�̗v�f��
	// index :       ���_�̃C���f�b�N�X���i�[�����z��
	SolidShape(GLint size, GLsizei vertexCount, const Object::Vertex* vertex)
		:Shape(size, vertexCount, vertex)
	{

	}

	//--- �`��̎��s ---//
	virtual void execute() const
	{
		// �O�p�`�ŕ`�悷��
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
};
