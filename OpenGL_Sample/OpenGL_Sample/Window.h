#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//=== �E�B���h�E�֘A�̏��� ===//
class Window
{
private:
    
    // �J�����E�B���h�E�̃n���h���i���ʎq�j
	GLFWwindow* const window;

    // �E�B���h�E�̃T�C�Y
    GLfloat size[2];

    // ���[���h���W�ɕϊ�����f�o�C�X���W�n�̊g�嗦
    GLfloat scale;

    // �}�`�̐��K���f�o�C�X���W�n��ł̈ʒu�i�����l�̓R���X�g���N�^�� 0 �ɐݒ�j
    GLfloat location[2];

    // �L�[�{�[�h�̏��
    int KeyStatus;

public:

	//--- �R���X�g���N�^ ---//
    Window(int width = 640, int height = 480, const char* title = "Hello!")
        : window(glfwCreateWindow(width, height, title, NULL, NULL))
        , scale(100.0f)
        , KeyStatus(GLFW_RELEASE)
    {
        if (window == NULL)
        {
            // �E�B���h�E���쐬�ł��Ȃ�����
            std::cerr << "Can't create GLFW window." << std::endl;
            exit(1);
        }

        // ���݂̃E�B���h�E�������Ώۂɂ���
        glfwMakeContextCurrent(window);

        // GLEW ������������
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            // GLEW �̏������Ɏ��s����
            std::cerr << "Can't initialize GLFW" << std::endl;
            exit(1);
        }

        // ���������^�C�~���O��҂�
        glfwSwapInterval(1);

        // �E�B���h�E�̃T�C�Y�ύX���ɌĂяo�������̓o�^
        glfwSetWindowSizeCallback(window, resize);

        // �}�E�X�E�z�C�[�����쎞�ɌĂяo�������̓o�^
        glfwSetScrollCallback(window, wheel);

        // �L�[�{�[�h���쎞�ɌĂяo�������̓o�^
        glfwSetKeyCallback(window, keyboard);

        // ���̃C���X�^���X�� this �|�C���^���L�^���Ă���
        glfwSetWindowUserPointer(window, this);

        // �J�����E�B���h�E�̏����ݒ�
        resize(window, width, height);

        // �}�`�̐��K���f�o�C�X���W�n��ł̈ʒu�̏�����
        location[0] = location[1] = 0.0f;
    }

    //--- �f�X�g���N�^ ---//
    virtual ~Window()
    {
        glfwDestroyWindow(window);
    }

    //--- �E�B���h�E�����ׂ����𔻒肷�� ---//
    int shouldClose() const
    {
        return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
    }

    //--- �J���[�o�b�t�@�����ւ��ăC�x���g�����o�� ---//
    void swapBuffers()
    {
        // �J���[�o�b�t�@�����ւ���
        glfwSwapBuffers(window);

        // �C�x���g�����o��
        glfwPollEvents();

        // �L�[�{�[�h�̏�Ԃ𒲂ׂ�
        if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
        {
            location[0] -= 2.0f / size[0];
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
        {
            location[0] += 2.0f / size[0];
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
        {
            location[1] -= 2.0f / size[0];
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
        {
            location[1] += 2.0f / size[0];
        }

        // �}�E�X�̍��{�^���̏�Ԃ𒲂ׂ�
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
        {
            // �}�E�X�̍��{�^����������Ă�����A�}�E�X�J�[�\���̈ʒu���擾����
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            // �}�E�X�J�[�\���̐��K���f�o�C�X���W�n��ł̈ʒu�����߂�
            location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
            location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
        }
    }

    //--- �c��������o�� ---//
    const GLfloat *getSize() const { return size; }

    //--- ���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦�����o�� ---//
    GLfloat getScale() const { return scale; }

    //--- �ʒu�����o�� ---//
    const GLfloat* getLocation() { return location; }

    //--- �E�B���h�E�̃T�C�Y�ύX���̏��� ---//
    static void resize(GLFWwindow* const window, int width, int height)
    {
        // �E�B���h�E�S�̂��r���[�|�[�g�ɂ���
        glViewport(0, 0, width, height);

        // ���̃C���X�^���X�� this �|�C���^�𓾂�
        Window* const
            instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

        if (instance != NULL)
        {
            // �J�����E�B���h�E�̃T�C�Y��ۑ�����
            instance->size[0] = static_cast<GLfloat>(width);
            instance->size[1] = static_cast<GLfloat>(height);
        }
    }

    //--- �E�B���h�E�̃T�C�Y�ύX���̏��� ---//
    static void wheel(GLFWwindow* window, double x, double y)
    {
        // ���̃C���X�^���X�� this �|�C���^�𓾂�
        Window* const
            instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

        if (instance != NULL)
        {
            // ���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦���X�V����
            instance->scale += static_cast<GLfloat>(y);
        }
    }

    static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        // ���̃C���X�^���X�� this �|�C���^�𓾂�
        Window* const
            instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

        if (instance != NULL)
        {
            // �L�[�̏�Ԃ�ۑ�����
            instance->KeyStatus = action;
        }
    }
};
