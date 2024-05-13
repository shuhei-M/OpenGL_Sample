#pragma once

#include <array>
#include <GL/glew.h>

//=== �}�`�f�[�^ ===//
class Object
{
private:

	// ���_�z��I�u�W�F�N�g��
	GLuint vao;

	// ���_�o�b�t�@�E�I�u�W�F�N�g��
	GLuint vbo;

	// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
	GLuint ibo;

public:

	//--- ���_���� ---//
	struct Vertex
	{
		// �ʒu
		GLfloat position[3];

		// �@��
		GLfloat normal[3];
	};

	//--- �R���X�g���N�^ ---//
	// size :        ���_�ʒu�̎���
	// vertexCount : ���_�̐�
	// vertex :      ���_�������i�[�����z��
	// indexCount :  ���_�C���f�b�N�X�̗v�f��
	// index :       ���_�̃C���f�b�N�X���i�[�����z��
	Object(GLint size, GLsizei vertexCount, const Vertex* vertex,
		GLsizei indexCount, const GLuint* index)
	{
		// ���_�z��I�u�W�F�N�g
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// ���_�o�b�t�@�I�u�W�F�N�g
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER,
			vertexCount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

		// ��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g�� in �ϐ�����Q�Ƃł���悤�ɂ���
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			static_cast<char*>(0) + sizeof vertex->position);
		glEnableVertexAttribArray(1);

		// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indexCount * sizeof(GLuint), index, GL_STATIC_DRAW);
	}

	//--- �f�X�g���N�^ ---//
	virtual ~Object()
	{
		// ���_�z��I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &vao);

		// ���_�o�b�t�@�E�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &vbo);

		// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &ibo);
	}

private:

	//--- �R�s�[�R���X�g���N�^�ɂ��R�s�[�֎~ ---//
	Object(const Object& o);

	//--- ����ɂ��R�s�[�֎~ ---//
	Object& operator=(const Object& o);

public:

	//--- ���_�z��I�u�W�F�N�g�̌��� ---//
	void bind() const
	{
		// �`�悷�钸�_�z��I�u�W�F�N�g���w�肷��
		glBindVertexArray(vao);
	}
};
