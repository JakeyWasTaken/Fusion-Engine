#pragma once
#include "FusionCore.h"

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

float clamp(float n, float lower, float upper)
{
	n = (n > lower) * n + !(n > lower) * lower;
	return (n < upper) * n + !(n < upper) * upper;
}

namespace Fusion
{
	class Camera
	{
	public:
		Camera(glm::vec3 position, Window window)
		{
			m_position = position;
#if FS_LOCK_MOUSE_ON_STARTUP
			glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

			// Update camera vectors on startup so we dont have to move mouse to get visuals
			m_LockedMouse = true;
			ProcessMouse(0, 0);
			m_LockedMouse = false;
			GetViewMat4();
		}

		void ProcessInput(Window window)
		{
			static bool CameraSwitch = false;
			static bool MouseStateSwitch = false;

			float scaledSpeed = m_speed * DeltaTime;
			GLFWwindow* glfwWindow = window.GetWindow();

			if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
				m_position += scaledSpeed * m_direction;
			if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
				m_position += -scaledSpeed * m_direction;
			if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
				m_position += m_right * scaledSpeed;
			if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
				m_position += -m_right * scaledSpeed;
			if (glfwGetKey(glfwWindow, GLFW_KEY_E) == GLFW_PRESS)
				m_position += m_up * scaledSpeed;
			if (glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS)
				m_position += -m_up * scaledSpeed;
			if (glfwGetKey(glfwWindow, GLFW_KEY_M) == GLFW_PRESS)
				MouseStateSwitch = true;
			if (glfwGetKey(glfwWindow, GLFW_KEY_C) == GLFW_PRESS)
				CameraSwitch = true;

			if (glfwGetKey(glfwWindow, GLFW_KEY_C) == GLFW_RELEASE && CameraSwitch == true)
			{
				CameraSwitch = false;
				m_Perspective = !m_Perspective;
			}

			if (glfwGetKey(glfwWindow, GLFW_KEY_M) == GLFW_RELEASE && MouseStateSwitch == true)
			{
				m_LockedMouse = !m_LockedMouse;
				MouseStateSwitch = false;

				if (m_LockedMouse)
					glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				else
					glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}

		void ProcessScroll(Window window, double yOffset)
		{
			if (glfwGetMouseButton(window.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				m_speed += yOffset;
				m_speed = clamp(m_speed, 1, 100);
				printf("[CAMERA] New move speed: %f\n", m_speed);
			}
			else
			{
				m_FOV += -yOffset;
				m_FOV = clamp(m_FOV, FS_MIN_FOV, FS_MAX_FOV);
				printf("[CAMERA] New field of view: %f\n", m_FOV);
			}
		}

		void ProcessMouse(double deltaX, double deltaY)
		{
			if (!m_LockedMouse)
				return;

			m_yaw += deltaX;
			m_pitch += deltaY;

			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			direction.y = sin(glm::radians(m_pitch));
			direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
			m_direction = glm::normalize(direction);
		}

		glm::mat4 GetViewMat4()
		{
			m_right = glm::normalize(glm::cross(up, m_direction));
			m_up = glm::cross(m_direction, m_right);
			return glm::lookAt(m_position, m_position + m_direction, m_up);
		}

		glm::mat4 GetCameraMat4(Window window)
		{
			if (m_Perspective)
			{
				float aspect = (float)window.GetWidth() / (float)window.GetHeight();

				if (aspect != aspect) // prevent assertion when minimized (checks if NaN)
				{
					return glm::mat4(1.0f);
				}

				return glm::perspective(glm::radians(GetFov()), aspect, FS_NEAR_PLANE, FS_FAR_PLANE);
			}
			//return glm::ortho<float>(0.0f, window.GetWidth(), 0.0f, window.GetHeight(), 0.0f - FS_NEAR_PLANE, FS_FAR_PLANE);
		}

		float GetFov()
		{
			return m_FOV;
		}

		bool IsMouseLocked() const { return m_LockedMouse; }

		glm::vec3 GetPosition()
		{
			return m_position;
		}

		glm::vec3 GetLookVector()
		{
			return m_direction;
		}

	private:
		float m_speed = 2.5f;
		float m_yaw = 0;
		float m_pitch = 0;
		float m_FOV = FS_FIELD_OF_VIEW;
		bool m_LockedMouse = FS_LOCK_MOUSE_ON_STARTUP;
		bool m_Perspective = FS_PERSPECTIVE_CAMERA;
		glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 m_direction = glm::normalize(m_position - glm::vec3(0.0f, 0.0f, 0.0f));
		glm::vec3 m_right = glm::normalize(glm::cross(up, m_direction));
		glm::vec3 m_up = glm::cross(m_direction, m_right);
	};
}