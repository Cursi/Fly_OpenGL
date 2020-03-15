#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace Laborator
{
	class Tema2Camera
	{
		public:
			Tema2Camera()
			{
				position = glm::vec3(0, 2, 5);
				forward = glm::vec3(0, 0, -1);
				up		= glm::vec3(0, 1, 0);
				right	= glm::vec3(1, 0, 0);
				distanceToTarget = 2;
			}

			Tema2Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				Set(position, center, up);
			}

			~Tema2Camera()
			{ }

			// Update Tema2Camera
			void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				this->position = position;
				forward = glm::normalize(center-position);
				right	= glm::cross(forward, up);
				this->up = glm::cross(right,forward);
			}

			void MoveForward(float distance)
			{
				glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
				// movement will keep the Tema2Camera at the same height always
				// Example: If you rotate up/down your head and walk forward you will still keep the same relative distance (height) to the ground!
				// Translate the Tema2Camera using the DIR vector computed from forward
			}

			void TranslateForward(float distance)
			{
				position += forward * distance;
				// TODO : Translate the Tema2Camera using the "forward" vector
			}

			void TranslateUpword(float distance)
			{
				position += up * distance;
				// TODO : Translate the Tema2Camera using the up vector
			}

			void TranslateRight(float distance)
			{
				position += right * distance;

				// TODO
				// Translate the Tema2Camera using the "right" vector
				// Usually translation using Tema2Camera "right' is not very useful because if the Tema2Camera is rotated around the "forward" vector 
				// translation over the right direction will have an undesired effect; the Tema2Camera will get closer or farther from the ground
				// Using the projected right vector (onto the ground plane) makes more sense because we will keep the same distance from the ground plane
			}

			void RotateFirstPerson_OX(float angle)
			{
				glm::vec3 myVector = glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1);
				forward = glm::normalize(glm::vec3(myVector));

				up = glm::cross(right, forward);

				// TODO
				// Compute the new "forward" and "up" vectors
				// Attention! Don't forget to normalize the vectors
				// Use glm::rotate()
			}

			void RotateFirstPerson_OY(float angle)
			{
				glm::vec4 myVector = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
				forward = glm::normalize(glm::vec3(myVector));
				
				myVector = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
				right = glm::normalize(glm::vec3(myVector));

				up = glm::cross(right, forward);

				// TODO
				// Compute the new "forward", "up" and "right" vectors
				// Don't forget to normalize the vectors
				// Use glm::rotate()
			}

			void RotateFirstPerson_OZ(float angle)
			{
				glm::vec3 myVector = glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(right, 1);
				right = glm::normalize(glm::vec3(myVector));

				up = glm::cross(right, forward);
				// TODO
				// Compute the new Right and Up, Forward stays the same
				// Don't forget to normalize the vectors
			}

			void RotateThirdPerson_OX(float angle)
			{
				TranslateForward(distanceToTarget);
				RotateFirstPerson_OX(angle);
				TranslateForward(-distanceToTarget);

				// TODO
				// Rotate the Tema2Camera in Third Person mode - OX axis
				// Use distanceToTarget as translation distance
			}

			void RotateThirdPerson_OY(float angle)
			{
				TranslateForward(distanceToTarget);
				RotateFirstPerson_OY(angle);
				TranslateForward(-distanceToTarget);

				// TODO
				// Rotate the Tema2Camera in Third Person mode - OY axis
			}

			void RotateThirdPerson_OZ(float angle)
			{
				TranslateForward(distanceToTarget);
				RotateFirstPerson_OZ(angle);
				TranslateForward(-distanceToTarget);

				// TODO
				// Rotate the Tema2Camera in Third Person mode - OZ axis
			}

			glm::mat4 GetViewMatrix()
			{
				// Returns the View Matrix
				return glm::lookAt(position, position + forward, up);
			}

			glm::vec3 GetTargetPosition()
			{
				return position + forward * distanceToTarget;
			}

		public:
			float distanceToTarget;
			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 right;
			glm::vec3 up;
		};
}