#include "Model.h"
#include "Skybox.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;


float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

std::string CubemapFolder = "lake";
std::string CubeMapTextureFaces[6] =
{
	"./textures/skybox/" + CubemapFolder + "/right.jpg",
	"./textures/skybox/" + CubemapFolder + "/left.jpg",
	"./textures/skybox/" + CubemapFolder + "/top.jpg",
	"./textures/skybox/" + CubemapFolder + "/bottom.jpg",
	"./textures/skybox/" + CubemapFolder + "/front.jpg",
	"./textures/skybox/" + CubemapFolder + "/back.jpg"
};

int main() {

	if (!glfwInit()) {
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Project", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);


	if (glewInit() != GLEW_OK) {
		return -1;
	}


	glEnable(GL_DEPTH_TEST);

	Shader lightShader("./shaders/light-vertex.glsl", "./shaders/light-fragment.glsl");

	Shader BasicProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl","./shaders/explode-geometry.glsl");
	Shader ReflectionShaderProgram("./shaders/vertex.glsl", "./shaders/reflect-fragment.glsl", "./shaders/explode-geometry.glsl");

	Shader RefractionShaderProgram("./shaders/vertex.glsl", "./shaders/refract-fragment.glsl", "./shaders/explode-geometry.glsl");

	Shader FresnalRefractionShaderProgram("./shaders/vertex.glsl", "./shaders/fresnal-fragment.glsl", "./shaders/explode-geometry.glsl");
	
	Shader ChromaticDispersionShaderProgram("./shaders/vertex.glsl", "./shaders/chromatic-fragment.glsl", "./shaders/explode-geometry.glsl");

	Shader SkyboxShaderProgram("./shaders/skybox-vertex.glsl", "./shaders/skybox-fragment.glsl");

	std::vector<std::vector<Texture>> PlaneTextures
	{
		{Texture("./textures/diffuse.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("./textures/specular.png", "specular", 1, GL_R, GL_UNSIGNED_BYTE)},
		{Texture("./textures/diffuse.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("./textures/specular.png", "specular", 1, GL_R, GL_UNSIGNED_BYTE)},
	
		{Texture("./textures/diffuse.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("./textures/specular.png", "specular", 1, GL_R, GL_UNSIGNED_BYTE)},

		{Texture("./textures/diffuse.png", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("./textures/specular.png", "specular", 1, GL_R, GL_UNSIGNED_BYTE)}
		/*
		*/
	};

	//Main Light
	//glm::vec4 lightColor = glm::vec4(0.66f, 0.75f, 1.0f, 1.0f);
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(0.0f, 4.0f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPosition);
	lightModel = glm::scale(lightModel, glm::vec3(1.0f, 1.0f, 1.0f)*0.2f);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	SkyboxShaderProgram.Activate();
	glUniform1i(glGetUniformLocation(SkyboxShaderProgram.ID, "skybox"), 0);

	BasicProgram.Activate();
	glUniform1i(glGetUniformLocation(BasicProgram.ID, "skybox"), 0);

	ReflectionShaderProgram.Activate();
	glUniform1i(glGetUniformLocation(ReflectionShaderProgram.ID, "skybox"), 0);

	RefractionShaderProgram.Activate();
	glUniform1i(glGetUniformLocation(RefractionShaderProgram.ID, "skybox"), 0);

	FresnalRefractionShaderProgram.Activate();
	glUniform1i(glGetUniformLocation(FresnalRefractionShaderProgram.ID, "skybox"), 0);

	ChromaticDispersionShaderProgram.Activate();
	glUniform1i(glGetUniformLocation(ChromaticDispersionShaderProgram.ID, "skybox"), 0);

	Model Light("./models/ball.obj", PlaneTextures);
	Light.model = lightModel;

	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 1.0f, 2.0f));

	const char modelName[] = "./models/teapot.obj";

	std::cout << "\nBasic: " << std::endl;
	Model Basic(modelName, PlaneTextures);
	glm::vec3 BasicPosition = glm::vec3(-10.0f, 0.0f, 0.0f);
	glm::mat4 Basic_model = glm::mat4(1.0f);
	Basic_model = glm::translate(Basic_model, BasicPosition);
	Basic_model = glm::scale(Basic_model, glm::vec3(1.0f, 1.0f, 1.0f) * 0.7f);
	Basic.model = Basic_model;
	Basic.UpdateLight(BasicProgram, lightColor, lightPosition);

	std::cout << "\nReflect: " << std::endl;
	Model Reflect(modelName, PlaneTextures);
	glm::vec3 ReflectPosition = glm::vec3(-5.0f, 0.0f, 0.0f);
	glm::mat4 Reflect_model = glm::mat4(1.0f);
	Reflect_model = glm::translate(Reflect_model, ReflectPosition);
	Reflect_model = glm::scale(Reflect_model, glm::vec3(1.0f, 1.0f, 1.0f) * 0.7f);
	Reflect.model = Reflect_model;
	Reflect.UpdateLight(ReflectionShaderProgram , lightColor, lightPosition);

	std::cout << "\nRefract: " << std::endl;
	Model Refract(modelName, PlaneTextures);
	glm::vec3 RefractPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 Refract_model = glm::mat4(1.0f);
	Refract_model = glm::translate(Refract_model, RefractPosition);
	Refract_model = glm::scale(Refract_model, glm::vec3(1.0f, 1.0f, 1.0f) * 0.7f);
	Refract.model = Refract_model;
	Refract.UpdateLight(RefractionShaderProgram, lightColor, lightPosition);

	std::cout << "\nFesnal Refract: " << std::endl;
	Model Fresnal(modelName, PlaneTextures);
	glm::vec3 FresnalPosition = glm::vec3(5.0f, 0.0f, 0.0f);
	glm::mat4 Fresnal_model = glm::mat4(1.0f);
	Fresnal_model = glm::translate(Fresnal_model, FresnalPosition);
	Fresnal_model = glm::scale(Fresnal_model, glm::vec3(1.0f, 1.0f, 1.0f) * 0.7f);
	Fresnal.model = Fresnal_model;
	Fresnal.UpdateLight(FresnalRefractionShaderProgram, lightColor, lightPosition);

	std::cout << "\nChromatic Dispersion: " << std::endl;
	Model Chromatic(modelName, PlaneTextures);
	glm::vec3 ChromaticPosition = glm::vec3(10.0f, 0.0f, 0.0f);
	glm::mat4 Chromatic_model = glm::mat4(1.0f);
	Chromatic_model = glm::translate(Chromatic_model, ChromaticPosition);
	Chromatic_model = glm::scale(Chromatic_model, glm::vec3(1.0f, 1.0f, 1.0f) * 0.7f);
	Chromatic.model = Chromatic_model;
	Chromatic.UpdateLight(ChromaticDispersionShaderProgram, lightColor, lightPosition);

	// Skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int CubeMapTexture;
	glGenTextures(1, &CubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//Completed Skybox

	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, numberOfChannels;
		unsigned char* data = stbi_load(CubeMapTextureFaces[i].c_str(), &width, &height, &numberOfChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
			std::cout << " Loaded Texture For Skybox : " << CubeMapTextureFaces[i].c_str() << std::endl;
		}
		else
		{
			std::cout << "Failed to load texture: " << CubeMapTextureFaces[i] << std::endl;
			stbi_image_free(data);
		}
	}

    while (!glfwWindowShouldClose(window)) {
		
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.UpdateMatrix(45.0f, 0.1f, 1000.0f);
		
		Basic.model = glm::rotate(Basic.model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Reflect.model = glm::rotate(Reflect.model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Refract.model = glm::rotate(Refract.model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Fresnal.model = glm::rotate(Fresnal.model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Chromatic.model = glm::rotate(Chromatic.model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		Basic.UpdateCamera(BasicProgram, camera);
		Basic.UpdateLight(BasicProgram, lightColor, lightPosition);

		Reflect.UpdateCamera(ReflectionShaderProgram, camera);
		Reflect.UpdateLight(ReflectionShaderProgram, lightColor, lightPosition);

		Refract.UpdateCamera(RefractionShaderProgram, camera);
		Refract.UpdateLight(RefractionShaderProgram, lightColor, lightPosition);

		Fresnal.UpdateCamera(FresnalRefractionShaderProgram, camera);
		Fresnal.UpdateLight(FresnalRefractionShaderProgram, lightColor, lightPosition);

		Chromatic.UpdateCamera(ChromaticDispersionShaderProgram, camera);
		Chromatic.UpdateLight(ChromaticDispersionShaderProgram, lightColor, lightPosition);

		lightShader.Activate();
		camera.Matrix(lightShader, "CameraMatrix");

		Basic.Draw(BasicProgram, camera);
		Reflect.Draw(ReflectionShaderProgram, camera);
		Refract.Draw(RefractionShaderProgram, camera);
		Fresnal.Draw(FresnalRefractionShaderProgram, camera);
		Chromatic.Draw(ChromaticDispersionShaderProgram, camera);

		//Skybox
		glDepthFunc(GL_LEQUAL);
		SkyboxShaderProgram.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(SkyboxShaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyboxShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		//Skybox End

		glfwSwapBuffers(window);
		glfwPollEvents();
    }

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}
