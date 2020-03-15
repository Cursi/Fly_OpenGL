#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"

#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <vector>

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

		// Clouds
		//-----------------------------------------------------------------------------------
		struct Cloud
		{
			Mesh* mesh;
			glm::vec3 position;
			float rotationAngle;
		};

		std::vector<Cloud> clouds;

		unsigned long long numberOfClouds = 0;
		//-----------------------------------------------------------------------------------

		// Obstacles
		//-----------------------------------------------------------------------------------
		struct Obstacle
		{
			Mesh* mesh;
			glm::vec3 position;
			float rotationAngle;
		};

		std::vector<Obstacle> obstacles;

		unsigned long long numberOfObstacles = 0;
		//-----------------------------------------------------------------------------------


		// Fuels
		//-----------------------------------------------------------------------------------
		struct Fuel
		{
			Mesh* mesh;
			glm::vec3 position;
			float rotationAngle;
		};

		std::vector<Fuel> fuels;

		unsigned long long numberOfFuels = 0;
		//-----------------------------------------------------------------------------------

	private:
		void GenerateCloud();
		Mesh* CreateCloud(std::string name);

		void GenerateObstacle();
		Mesh* CreateObstacle(std::string name);

		void GenerateFuel(int x);
		Mesh* CreateFuel(std::string name);

		bool IsCollision(glm::vec3 itemPosition, bool isObstacle);

		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int isSeaObject);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		// Lab 5 - Camere
		//-----------------------------------------------------------------------------------
		Laborator::Tema2Camera *camera;
		glm::mat4 projectionMatrix;
		bool isFirstPersonCamera;
		//-----------------------------------------------------------------------------------

		// Lab 8 - Iluminare Phong
		//-----------------------------------------------------------------------------------
		glm::vec3 lightPosition;
		glm::vec3 lightDirection;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		int spotlightWanted;
		//-----------------------------------------------------------------------------------

		// Plane
		//-----------------------------------------------------------------------------------
		glm::vec3 planePosition;
		float planeRotationX;
		float propellerRotationX;
		float planeSpeed;
		float planeFuel;
		float planeRadius;
		int numberOfLifes;
		//-----------------------------------------------------------------------------------
};