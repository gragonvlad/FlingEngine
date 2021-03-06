#pragma once
#include "FlingMath.h"

namespace Fling
{
	/**
	 * @brief Base class for camera, meant to be overridden
	 * 
	 */
	class Camera
	{
	public:
		Camera() :
			m_nearPlane(0.1f),
			m_farPlane(1000.0f),
			m_fieldOfView(glm::radians(45.0f)),
			m_aspectRatio(1.6180f) //golden ratio
		{
		}

		virtual ~Camera() = default;
		virtual void Update(float dt) = 0;

		/**
		 * @brief Gets the near plane of the view frustrum
		 * 
		 * @return const float& m_nearPlane
		 */
		const float GetNearPlane() const { return m_nearPlane; }
		void SetNearPlane(const float& nearPlane) { m_nearPlane = nearPlane; }

		/**
		 * @brief Gets the far plane of the view frustrum
		 * 
		 * @return const float& m_farPlane
		 */
		const float GetFarPlane() const { return m_farPlane; }
		void SetFarPlane(const float& farPlane) { m_farPlane = farPlane; }

		/**
		 * @brief Gets the field of view angle from the view frustrum
		 * 
		 * @return const float& m_fieldOfView
		 */
		const float GetFieldOfView() const { return m_fieldOfView; }
		void SetFieldOfView(const float& fieldOfView) { m_fieldOfView = fieldOfView; }

		const glm::vec3& GetPosition() const { return m_position; }
		const glm::vec3& GetRotation() const { return m_rotation; }
		const float GetSpeed() const { return m_speed; }
		const float GetAspectRatio() const { return m_aspectRatio; }

		/**
		 * @brief Gets the view matrix created by the current camera position and rotation
		 * 
		 * @return const glm::mat4& viewMatrix
		 */
		const glm::mat4& GetViewMatrix() const {return m_viewMatrix; }

		/**
		 * @brief Gets the projection matrix used by camera
		 * 
		 * @return const glm::mat4& projectionMatrix
		 */
		const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }

		float GetGamma() const { return m_Gamma; }
		void SetGamma(float t_Gam) { m_Gamma = t_Gam; }

		float GetExposure() const { return m_Exposure; }
		void SetExposure(float t_Val) { m_Exposure = t_Val; }

	protected:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		glm::vec3 m_position;
		float m_speed; //padding 12 + 4 = 16
		
		glm::vec3 m_rotation;
		float m_aspectRatio;

		float m_nearPlane;
		float m_farPlane;
		float m_fieldOfView;

		float m_Gamma = 2.2f;
		float m_Exposure = 4.5f;
	};
} //namespace fling