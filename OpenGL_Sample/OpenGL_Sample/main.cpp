//===  ===//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "Window.h"
#include "Matrix.h"
#include "Vector.h"
#include "Shape.h"
#include "ShapeIndex.h"
#include "SolidShapeIndex.h"
#include "SolidShape.h"
#include "Uniform.h"
#include "Material.h"

//=== �V�F�[�_�I�u�W�F�N�g�̃R���p�C�����ʂ�\������ ===//
// shader: �V�F�[�_�I�u�W�F�N�g��
// str: �R���p�C���G���[�����������ꏊ������������
GLboolean printShaderInfoLog(GLuint shader, const char* str)
{
	// �R���p�C�����ʂ�\������
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cerr << "Compile Error in " << str << std::endl;
	}

	// �V�F�[�_�[�̃R���p�C�����̃��O�̒������擾����
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1)
	{
		// �V�F�[�_�[�̃R���p�C�����̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

//=== �v���O�����I�u�W�F�N�g�̃����N���ʂ�\������ ===//
// program�F�v���O�����I�u�W�F�N�g��
GLboolean printProgramInfoLog(GLuint program)
{
	// �����N���ʂ��擾����
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cerr << "Link Error." << std::endl;
	}

	// �V�F�[�_�[�����N�̃��O�̒������擾����
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1)
	{
		// �V�F�[�_�[�̃����N���̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

//=== �v���O�����I�u�W�F�N�g���쐬���� ===//
// vsrc: �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̕�����
// fsrc: �t���O�����g�V�F�[�_�̃\�[�X�v���O�����̕�����
GLuint createProgram(const char* vsrc, const char* fsrc)
{
	// ��̃v���O�����E�I�u�W�F�N�g�����
	const GLuint program(glCreateProgram());

	if (vsrc != NULL)
	{
		// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�E�I�u�W�F�N�g�����
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);

		// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�E�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if (printShaderInfoLog(vobj, "vertex shader"))
		{
			glAttachShader(program, vobj);
		}
		glDeleteShader(vobj);
	}

	if (fsrc != NULL)
	{
		// �t���O�����g�V�F�[�_�̃V�F�[�_�E�I�u�W�F�N�g�����
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		// �t���O�����g�V�F�[�_�̃V�F�[�_�E�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if (printShaderInfoLog(fobj, "fragment shader"))
		{
			glAttachShader(program, fobj);
		}
		glDeleteShader(fobj);
	}

	// �v���O�����I�u�W�F�N�g�ƃ����N����
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	// �쐬�����v���O�����I�u�W�F�N�g��Ԃ�
	if (printProgramInfoLog(program))
	{
		return program;
	}

	// �v���O�����E�I�u�W�F�N�g���쐬�ł��Ȃ���� 0 ��Ԃ�
	glDeleteProgram(program);
	return 0;
}

//=== �V�F�[�_�̃\�[�X�t�@�C����ǂݍ��񂾃�������Ԃ� ===//
// name: �V�F�[�_�̃\�[�X�t�@�C����
// buffer: �ǂݍ��񂾃\�[�X�t�@�C���̃e�L�X�g
bool readShaderSource(const char* name, std::vector<GLchar>& buffer)
{
	// �t�@�C������ NULL ������
	if (name == NULL) return false;

	// �\�[�X�t�@�C�����J��
	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		// �J���Ȃ�����
		std::cerr << "Error: Can't open source file: " << name << std::endl;
		return false;
	}

	// �t�@�C���̖����Ɉړ������݈ʒu�i���t�@�C���T�C�Y�j�𓾂�
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	// �t�@�C���T�C�Y�̃��������m��
	buffer.resize(length + 1);

	// �t�@�C����擪����ǂݍ���
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';

	if (file.fail())
	{
		// ���܂��ǂݍ��߂Ȃ�����
		std::cerr << "Error: Could not read souce file: " << name << std::endl;
		file.close();
		return false;
	}

	// �ǂݍ��ݐ���
	file.close();
	return true;
}

//=== �V�F�[�_�̃\�[�X�t�@�C����ǂݍ���Ńv���O�����I�u�W�F�N�g���쐬���� ===//
// vert: �o�[�e�b�N�X�V�F�[�_�̃\�[�X�t�@�C����
// frag: �t���O�����g�V�F�[�_�̃\�[�X�t�@�C����
GLuint loadProgram(const char* vert, const char* frag)
{
	// �V�F�[�_�̃\�[�X�t�@�C����ǂݍ���
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));

	// �v���O�����I�u�W�F�N�g���쐬����
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

int main()
{
	//--- GLFW ������������ ---//
	if (glfwInit() == GL_FALSE)
	{
		// �������Ɏ��s����
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	//--- �v���O�����I��������ǉ����� ---//
	// atexit(function)�F����function ���v���O�����I�����Ɏ��s����
	atexit(glfwTerminate);

	//--- OpenGL Version 3.2 Core Profile ��I������ ---//
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//--- �E�B���h�E���쐬���� ---//
	Window window;

	//--- �w�i�F���w�肷�� ---//
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	//--- �w�ʃJ�����O��L���ɂ��� ---//
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//--- �f�v�X�o�b�t�@��L���ɂ��� ---//
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	//--- �v���O�����I�u�W�F�N�g���쐬���� ---//
	const GLuint program(loadProgram("point.vert", "point.frag"));

	//--- uniform �ϐ��̏ꏊ���擾���� ---//
	const GLint modelViewLoc(glGetUniformLocation(program, "modelview"));
	const GLint projectionLoc(glGetUniformLocation(program, "projection"));
	const GLint normalMatrixLoc(glGetUniformLocation(program, "normalMatrix"));
	const GLint LposLoc(glGetUniformLocation(program, "Lpos"));
	const GLint LambLoc(glGetUniformLocation(program, "Lamb"));
	const GLint LdiffLoc(glGetUniformLocation(program, "Ldiff"));
	const GLint LspecLoc(glGetUniformLocation(program, "Lspec"));

	//--- uniform block �̏ꏊ���擾���� ---//
	const GLint materialLoc(glGetUniformBlockIndex(program, "Material"));

	//--- uniform block �̏ꏊ�� 0 �Ԃ̌����|�C���g�Ɍ��т��� ---//
	glUniformBlockBinding(program, materialLoc, 0);

	//--- ���̕����� ---//
	const int slices(16), stacks(8);

	//--- ���_��������� ---//
	std::vector<Object::Vertex> solidSphereVertex;

	for (int j = 0; j <= stacks; ++j)
	{
		const float t(static_cast<float>(j) / static_cast<float>(stacks));
		const float y(cos(3.141593f * t)), r(sin(3.141593f * t));

		for (int i = 0; i <= slices; ++i)
		{
			const float s(static_cast<float>(i) / static_cast<float>(slices));
			const float z(r * cos(6.283185f * s)), x(r * sin(6.283185f * s));

			// ���_����
			const Object::Vertex v = { x, y, z, x, y, z };

			// ���_������ǉ�����
			solidSphereVertex.emplace_back(v);
		}
	}

	// �C���f�b�N�X�����
	std::vector<GLuint> solidSphereIndex;

	for (int j = 0; j < stacks; ++j)
	{
		const int k((slices + 1) * j);

		for (int i = 0; i < slices; ++i)
		{
			// ���_�̃C���f�b�N�X
			const GLuint k0(k + i);
			const GLuint k1(k0 + 1);
			const GLuint k2(k1 + slices);
			const GLuint k3(k2 + 1);

			// �����̎O�p�`
			solidSphereIndex.emplace_back(k0);
			solidSphereIndex.emplace_back(k2);
			solidSphereIndex.emplace_back(k3);

			// �E��̎O�p�`
			solidSphereIndex.emplace_back(k0);
			solidSphereIndex.emplace_back(k3);
			solidSphereIndex.emplace_back(k1);
		}
	}

	//--- �}�`�f�[�^���쐬���� ---//
	std::unique_ptr<const Shape> shape(new SolidShapeIndex(3,
		static_cast<GLsizei>(solidSphereVertex.size()), solidSphereVertex.data(),
		static_cast<GLsizei>(solidSphereIndex.size()), solidSphereIndex.data()));

	//--- �����f�[�^ ---//
	static constexpr int Lcount(2);
	static constexpr Vector Lpos[] = { 0.0f, 0.0f, 5.0f, 1.0f, 8.0f, 0.0f, 0.0f, 1.0f };
	static constexpr GLfloat Lamb[] = { 0.2f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f };
	static constexpr GLfloat Ldiff[] = { 1.0f, 0.5f, 0.5f, 0.9f, 0.9f, 0.9f };
	static constexpr GLfloat Lspec[] = { 1.0f, 0.5f, 0.5f, 0.9f, 0.9f, 0.9f };

	//--- �F�f�[�^ ---//
	static constexpr Material color[] =
	{
		// Kamb Kdiff Kspec Kshi
		{ 0.6f, 0.6f, 0.2f, 0.6f, 0.6f, 0.2f, 0.3f, 0.3f, 0.3f, 30.0f },
		{ 0.1f, 0.1f, 0.5f, 0.1f, 0.1f, 0.5f, 0.4f, 0.4f, 0.4f, 60.0f }
	};

	const Uniform<Material> material(color, 2);

	//--- �^�C�}�[�� 0 �ɃZ�b�g ---//
	glfwSetTime(0.0);

	//--- �E�B���h�E���J���Ă���Ԃ͌J��Ԃ� ---//
	// glfwWindowShouldClose�F�����̃E�B���h�E�����K�v�����鎞�A
	while (window.shouldClose() == GL_FALSE)
	{
		// �E�B���h�E����������i�E�B���h�E��h��Ԃ��i�h��Ԃ��o�b�t�@�j�j
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// �V�F�[�_�v���O�����̎g�p�J�n
		glUseProgram(program);

		// �������e�ϊ��s������߂�
		const GLfloat* const size(window.getSize());
		const GLfloat fovy(window.getScale() * 0.01f);
		const GLfloat aspect(size[0] / size[1]);
		const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));

		// ���f���ϊ��s������߂�
		const GLfloat* const location(window.getLocation());
		const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 0.1f, 0.0f));
		const Matrix model(Matrix::translate(location[0], location[1], 0.0f) * r);

		// �r���[�ϊ��s������߂�
		const Matrix view(Matrix::lookat(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

		// �@���x�N�g���̕ϊ��s��̊i�[��
		GLfloat normalMatrix[9];

		// ���f���r���[�ϊ��s������߂�
		const Matrix modelView(view * model);

		// �@���x�N�g���̕ϊ��s������߂�
		modelView.getNormalMatrix(normalMatrix);

		// uniform �ϐ��ɒl��ݒ肷��
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelView.data());
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix);
		for (int i = 0; i < Lcount; i++)
		{
			glUniform4fv(LposLoc + i, 1, (view * Lpos[i]).data());
		}
		glUniform3fv(LambLoc, Lcount, Lamb);
		glUniform3fv(LdiffLoc, Lcount, Ldiff);
		glUniform3fv(LspecLoc, Lcount, Lspec);

		// �}�`��`�悷��
		material.select(0);   // �g�p���郆�j�t�H�[���o�b�t�@�E�I�u�W�F�N�g��I��
		shape->draw();

		// ��ڂ̃��f���r���[�ϊ��s������߂�
		const Matrix modelView1(modelView * Matrix::translate(0.0f, 0.0f, 3.0f));

		// ��ڂ̖@���x�N�g���̕ϊ��s������߂�
		modelView1.getNormalMatrix(normalMatrix);

		// uniform �ϐ��̒l��ݒ肷��
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelView1.data());
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix);

		// ��ڂ̐}�`��`�悷��
		material.select(1);   // �g�p���郆�j�t�H�[���o�b�t�@�E�I�u�W�F�N�g��I��
		shape->draw();

		// �J���[�E�o�b�t�@�����ւ��āA�C�x���g�����o��
		window.swapBuffers();
	}

	std::cout << "End" << std::endl;
}
