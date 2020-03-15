#include "Tema2.h"


#include <time.h>
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	/* initialize random seed: */
	srand(time(NULL));

	camera = new Laborator::Tema2Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	isFirstPersonCamera = false;

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sea");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "sea.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "plane.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("propeller");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "propeller.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("life1");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "star.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("life2");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "star.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("life3");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "star.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("fuelBar");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	for (int i = 0; i < 8; i++) 
	{
		GenerateCloud();
	}

	for (int i = 0; i < 4; i++)
	{
		GenerateObstacle();
	}

	for (int i = 0; i < 4; i++)
	{
		GenerateFuel(i * 12);
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 300.0f);

	// Create a shader
	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(10, 90, 10);
		lightDirection = glm::vec3(0, -1, 0);
		materialShininess = 10;
		materialKd = 0.6;
		materialKs = 0.25;
		spotlightWanted = 1;
	}

	{
		planePosition = glm::vec3(-2, 2, 0);
		planeRotationX = -90;
		planeRadius = 0.3f;
		planeSpeed = 0.24f;
		propellerRotationX = 0;

		planeFuel = 1;
		numberOfLifes = 3;
	}
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.91f, 0.73f, 0.68, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

Mesh* Tema2::CreateCloud(string name)
{
	Mesh* mesh = new Mesh(name);
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "cloud.fbx");
	return mesh;
}

void Tema2::GenerateCloud()
{
	string currentCloudID = "cloud" + to_string(++numberOfClouds);
	Mesh* currentMesh = CreateCloud(currentCloudID);

	glm::vec3 cloudPosition = glm::vec3(20 + rand() % 36, rand() % 5, -(rand() % 10) - 6);
	
	clouds.push_back(
	{
		currentMesh,
		cloudPosition,
		0		
	});
}


Mesh* Tema2::CreateObstacle(string name)
{
	Mesh* mesh = new Mesh(name);
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "obstacle.obj");
	return mesh;
}

void Tema2::GenerateObstacle()
{
	string currentObstacleID = "obstacle" + to_string(++numberOfObstacles);
	Mesh* currentMesh = CreateObstacle(currentObstacleID);

	glm::vec3 obstaclePosition = glm::vec3(5 + rand() % 10, -2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.7f - -2))), 0);

	obstacles.push_back(
	{
		currentMesh,
		obstaclePosition,
		0
	});
}

Mesh* Tema2::CreateFuel(string name)
{
	Mesh* mesh = new Mesh(name);
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tema2_Assets", "fuel.fbx");
	return mesh;
}

void Tema2::GenerateFuel(int x)
{
	float randomY = -1.3f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.7f - -1.3f)));
	int randomUP = rand() % 2;

	for (int i = 0; i < 5; i++) 
	{
		string currentFuelID = "fuel" + to_string(++numberOfFuels);
		Mesh* currentMesh = CreateFuel(currentFuelID);

		glm::vec3 fuelPosition = glm::vec3(5 + x + (float)i / 4, randomY - (randomUP == 1 ? 1 : -1) * ((float)i / 8), 0);

		fuels.push_back(
		{
			currentMesh,
			fuelPosition,
			0
		});
	}
}

bool Tema2::IsCollision(glm::vec3 itemPosition, bool isObstacle) 
{
	float itemRadius = isObstacle ? 0.1f : 0.05f;

	float distance = sqrt((planePosition.x - itemPosition.x) * (planePosition.x - itemPosition.x) +
		(planePosition.y - itemPosition.y) * (planePosition.y - itemPosition.y) +
		(planePosition.z - itemPosition.z) * (planePosition.z - itemPosition.z));

	return distance < (planeRadius + itemRadius);
}

void Tema2::Update(float deltaTimeSeconds)
{
	if (isFirstPersonCamera) camera->Set(glm::vec3(planePosition.x + 0.6f, planePosition.y, planePosition.z), glm::vec3(0, planePosition.y, 0), glm::vec3(0, 1, 0));
	else camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	{
		glm::mat4 modelMatrix = glm::mat4(1);

		if (isFirstPersonCamera)
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(planePosition.x + 2.7f, planePosition.y + 1.07f, planePosition.z + 1.8f));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
		}
		else
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(1.95f, 2.5f, 1));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(0), glm::vec3(0, 1, 0));
		}

		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f * planeFuel, 0.1f, 0.1f));
		RenderSimpleMesh(meshes["fuelBar"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.5f, 1, 0), 0);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);

		if (isFirstPersonCamera) 
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(planePosition.x + 2.7f, planePosition.y + 1.02f, planePosition.z - 1.95f));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
		} 
		else 
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.15f, 2.45f, 1));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(0), glm::vec3(0, 1, 0));
		}

		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["life1"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0.5f), 0);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		
		if (isFirstPersonCamera)
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(planePosition.x + 2.7f, planePosition.y + 1.02f, planePosition.z - 1.7f));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
		}
		else
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.9f, 2.45f, 1));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(0), glm::vec3(0, 1, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["life2"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0.5f), 0);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		
		if (isFirstPersonCamera)
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(planePosition.x + 2.7f, planePosition.y + 1.02f, planePosition.z - 1.45f));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
		}
		else
		{
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.65f, 2.45f, 1));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(0), glm::vec3(0, 1, 0));
		}

		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["life3"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0.5f), 0);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -4, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(1, 0, 0));

		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.015f, 0.005f));
		RenderSimpleMesh(meshes["sea"], shaders["ShaderTema2"], modelMatrix, glm::vec3(0.73f, 0.95f, 1), 1);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, planePosition);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(planeRotationX), glm::vec3(1, 0, 0));
		
		modelMatrix = glm::rotate(modelMatrix, RADIANS(180.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.02f));
		RenderSimpleMesh(meshes["plane"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 1), 0);

		planeFuel -= 0.008f * planeSpeed;
		cout << "Fuel: " << planeFuel << '\n';

		if (planeFuel < 0)
		{
			cout << "You ran out of fuel!\n";
			exit(0);
		}
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(planePosition.x + 0.51f, planePosition.y + 0.005f, 0));
		propellerRotationX += planeSpeed > 0.24f ? 36 : 18;
		modelMatrix = glm::rotate(modelMatrix, RADIANS(propellerRotationX), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(180.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.44f, 0.22f, 0.22f));
		RenderSimpleMesh(meshes["propeller"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 0.3f), 0);
	}

	{
		for (int i = 0; i < clouds.size(); i++) 
		{
			clouds[i].position.x -= planeSpeed;
			if (clouds[i].position.x < -20) clouds[i].position = glm::vec3(20 + rand() % 36, rand() % 5, -(rand() % 10) -6);

			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, clouds[i].position);
			clouds[i].rotationAngle += 2;
			modelMatrix = glm::rotate(modelMatrix, RADIANS(clouds[i].rotationAngle), glm::vec3(1, 0, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f, 0.7f, 3));
			RenderSimpleMesh(clouds[i].mesh, shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 1), 0);
		}
	}

	{
		for (int i = 0; i < obstacles.size(); i++)
		{
			obstacles[i].position.x -= 0.35f * planeSpeed;
			
			if (IsCollision(obstacles[i].position, true))
			{
				string currentLifeMeshID = "life";
				currentLifeMeshID += (char)(numberOfLifes + '0');
				meshes[currentLifeMeshID]->ClearData();

				numberOfLifes--;
				if (numberOfLifes == 0) exit(0);
			}
			
			if (IsCollision(obstacles[i].position, true) || obstacles[i].position.x < -5)
				obstacles[i].position = glm::vec3(5, -2 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.7f - -2))), 0);

			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, obstacles[i].position);
			obstacles[i].rotationAngle += 4;
			modelMatrix = glm::rotate(modelMatrix, RADIANS(obstacles[i].rotationAngle), glm::vec3(1, 1, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
			RenderSimpleMesh(obstacles[i].mesh, shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0), 0);
		}
	}

	{
		for (int i = 0; i < fuels.size(); i++)
		{
			fuels[i].position.x -= 0.25f * planeSpeed;

			if (IsCollision(fuels[i].position, false)) 
			{
				if(planeFuel < 0.9f) planeFuel += 0.1f;
				fuels[i].position.y = -10;
			}

			if (fuels[i].position.x < -5 && i % 5 == 4)
			{
				float randomY = -1.3f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.7f - -1.3f)));
				int randomUP = rand() % 2;

				for (int j = i - 4; j <= i; j++) 
				{
					fuels[j].position = glm::vec3(5 + i * 2 + (float)(i - j) / 4, randomY - (randomUP == 1 ? 1 : -1) * ((float)(i-j) / 8), 0);
				}
			} 
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, fuels[i].position);
			fuels[i].rotationAngle += 4;
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fuels[i].rotationAngle), glm::vec3(1, 1, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.0005f));
			RenderSimpleMesh(fuels[i].mesh, shaders["ShaderTema2"], modelMatrix, glm::vec3(0, 0.9f, 0.2f), 0);
		}
	}

	// Render the point light in the scene
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, lightPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
	}
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int isSeaObject)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	int elapsedTimeLocation = glGetUniformLocation(shader->program, "ElapsedTime");
	glUniform1f(elapsedTimeLocation, Engine::GetElapsedTime());

	int isSeaObjectLocation = glGetUniformLocation(shader->program, "isSeaObject");
	glUniform1i(isSeaObjectLocation, isSeaObject);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int spotlightWantedLocation = glGetUniformLocation(shader->program, "spotlightWanted");
	glUniform1i(spotlightWantedLocation, spotlightWanted);

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	//glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	//glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{

}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_S) planeSpeed = 0.48f;
	if (key == GLFW_KEY_C) isFirstPersonCamera = !isFirstPersonCamera;
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_S) planeSpeed = 0.24f;
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	planePosition.y += -deltaY * 0.0015f;
	planeRotationX += deltaY * 0.05f;
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
