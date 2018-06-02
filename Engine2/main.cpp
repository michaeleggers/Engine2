/*
Author: Michael Eggers
Course: Game Programming, COMP 4451, HKUST, Fall Term 2016
*/


#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>
#include <glm/gtx/vector_angle.hpp>

#include "display.h"
#include "AnimationShader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "ObjLoader.h"
#include "ObjLoader2.h"
#include "md2Model.h"
#include "Character.h"
#include "AnimatedEntity.h"

#define EPSILON 0.5f;

glm::vec3 gameCamPos = glm::vec3(0.0f, 4.0f, 10.0f);
glm::vec3 gameCamFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 gameCamUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 freeCamPos = glm::vec3(0.0f, 0.0f, 30.0f);
glm::vec3 freeCamFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 freeCamUp = glm::vec3(0.0f, 1.0f, 0.0f);

static Camera globalCam;
static unsigned int activeCam = 1; // 1: game cam, 2: free cam

// pitch and yaw
GLfloat pitch = 0.0f;
GLfloat yaw = 0.0f;
glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);

// frame for animation
int frame = 0;


GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
GLfloat deltaAnim = 0.0f;	// timer for animation

bool animIsRunning = false;	// check if animation is currently running

unsigned int keycodes[SDL_NUM_SCANCODES];

// function that moves the camera
void processCamera() {
	if (keycodes[SDL_SCANCODE_W]) {
		freeCamPos += direction * deltaTime * 0.05f;
	}
	if (keycodes[SDL_SCANCODE_S]) {
		freeCamPos -= direction * deltaTime * 0.05f;
	}
	if (keycodes[SDL_SCANCODE_A]) {
		freeCamPos -= glm::normalize(glm::cross(direction, freeCamUp)) * deltaTime * 0.05f;
	}
	if (keycodes[SDL_SCANCODE_D]) {
		freeCamPos += glm::normalize(glm::cross(direction, freeCamUp)) * deltaTime * 0.05f;
	}
	// pitch
	if (keycodes[SDL_SCANCODE_UP]) {
		pitch += 0.05f * deltaTime;
		if (pitch > 89.0f)
			pitch = 89.0f;
	}
	if (keycodes[SDL_SCANCODE_DOWN]) {
		pitch -= 0.05f * deltaTime;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	// yaw
	if (keycodes[SDL_SCANCODE_LEFT]) {
		yaw -= 0.05f * deltaTime;
	}
	if (keycodes[SDL_SCANCODE_RIGHT]) {
		yaw += 0.05f * deltaTime;
	}

	// update pitch and yaw
	direction.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	direction.y = glm::sin(glm::radians(pitch));
	direction.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
}

void processCharacter(Character& character) {
	bool keyHit = false;
	glm::vec3 newDirection(0.0f, 0.0f, 0.0f);

	if (keycodes[SDL_SCANCODE_D]) {							// right
		character.setAnimationState(RUN);
		newDirection += glm::vec3(1.0f, 0.0f, 0.0f);
		keyHit = true;
	}
	if (keycodes[SDL_SCANCODE_A]) {							// left
		character.setAnimationState(RUN);
		newDirection += glm::vec3(-1.0f, 0.0f, 0.0f);
		keyHit = true;
	}
	if (keycodes[SDL_SCANCODE_W]) {							// forward
		character.setAnimationState(RUN);
		newDirection += glm::vec3(0.0f, 0.0f, -1.0f);
		keyHit = true;
	}
	if (keycodes[SDL_SCANCODE_S]) {							// backwards
		character.setAnimationState(RUN);
		newDirection += glm::vec3(0.0f, 0.0f, 1.0f);
		keyHit = true;
	}
	if (keycodes[SDL_SCANCODE_SPACE]) {
		character.setAnimationState(ATTACK);
		newDirection = character.getDirection();
		keyHit = true;
	}
	if (!keyHit) {
		character.setAnimationState(IDLE);
		newDirection = character.getDirection();
	}

	//newDirection = glm::normalize(newDirection);
	character.setDirection(newDirection);
}

/* checks each Character in the scene if it is bumping into something. */
void collisionDetect(std::vector<Character*> entities) {
	for (int i = 0; i < entities.size(); ++i) {
		for (int j = i + 1; j < entities.size(); ++j) {
			if (entities[i]->getTransform().pos.x < entities[j]->getTransform().pos.x) { // player left from npc
				if (entities[i]->getTransform().pos.x + 0.001f + entities[i]->getBBox().getWidth(40) / 2 > entities[j]->getTransform().pos.x
					&& entities[i]->getTransform().pos.z < entities[j]->getTransform().pos.z + entities[j]->getBBox().getDepth(40) / 2
					&& entities[i]->getTransform().pos.z > entities[j]->getTransform().pos.z - entities[j]->getBBox().getDepth(40) / 2) {

					entities[i]->setVelocity(0.0f); // no contribution in x direction anymore
					entities[i]->getTransform().pos += -entities[i]->getDirection() * 0.1f; // set back a bit so the model doesn't get stuck in the obstacle
				}
				else {
					entities[i]->setVelocity(0.03f);
				}
			}
			if (entities[i]->getTransform().pos.x > entities[j]->getTransform().pos.x) { // player right from npc
				if (entities[i]->getTransform().pos.x - 0.001f - entities[i]->getBBox().getWidth(40) / 2 < entities[j]->getTransform().pos.x
					&& entities[i]->getTransform().pos.z < entities[j]->getTransform().pos.z + entities[j]->getBBox().getDepth(40) / 2
					&& entities[i]->getTransform().pos.z > entities[j]->getTransform().pos.z - entities[j]->getBBox().getDepth(40) / 2) {

					entities[i]->setVelocity(0.0f); // no contribution in x direction anymore
					entities[i]->getTransform().pos += -entities[i]->getDirection() * 0.1f; // set back a bit so the model doesn't get stuck in the obstacle
				}
				else {
					entities[i]->setVelocity(0.03f);
				}
			}
			if (entities[i]->getTransform().pos.z > entities[j]->getTransform().pos.z) { // player in front of npc
				if (entities[i]->getTransform().pos.z - 0.001f - entities[i]->getBBox().getDepth(40) / 2 < entities[j]->getTransform().pos.z
					&& entities[i]->getTransform().pos.x < entities[j]->getTransform().pos.x + entities[j]->getBBox().getWidth(40) / 2
					&& entities[i]->getTransform().pos.x > entities[j]->getTransform().pos.x - entities[j]->getBBox().getWidth(40) / 2) {

					entities[i]->setVelocity(0.0f); // no contribution in x direction anymore
					entities[i]->getTransform().pos += -entities[i]->getDirection() * 0.1f; // set back a bit so the model doesn't get stuck in the obstacle
				}
				else {
					entities[i]->setVelocity(0.03f);
				}
			}
			if (entities[i]->getTransform().pos.z < entities[j]->getTransform().pos.z) { // player behind of npc
				if (entities[i]->getTransform().pos.z + 0.001f + entities[i]->getBBox().getDepth(40) / 2 > entities[j]->getTransform().pos.z
					&& entities[i]->getTransform().pos.x < entities[j]->getTransform().pos.x + entities[j]->getBBox().getWidth(40) / 2
					&& entities[i]->getTransform().pos.x > entities[j]->getTransform().pos.x - entities[j]->getBBox().getWidth(40) / 2) {

					entities[i]->setVelocity(0.0f); // no contribution in x direction anymore
					entities[i]->getTransform().pos += -entities[i]->getDirection() * 0.1f; // set back a bit so the model doesn't get stuck in the obstacle
				}
				else {
					entities[i]->setVelocity(0.03f);
				}
			}
		}
	}
}

void computeAI(Character& character, Character& player) {
	character.setDirection(glm::normalize(player.getTransform().pos - character.getTransform().pos));
	character.getTransform().pos += deltaTime * character.getVelocity() * character.getDirection();

	float angle = glm::angle(glm::vec3(1.0f, 0.0f, 0.0f), character.getDirection());
	if (character.getDirection().z > 0) {
		angle = glm::angle(glm::vec3(-1.0f, 0.0f, 0.0f), character.getDirection());
		angle += glm::radians(180.0f);
	}
	character.getTransform().rot.y = angle;
}


int main(int argc, char** argv) {
	Display display(1024, 768, "Engine2");

	std::vector<std::string> shader_variables_md2 = { "tposition", "texCoord", "normal", "nextVertexFrame", "nextNormalFrame" };
	/* md2 shader for md2 models to animate them */
	AnimationShader md2Shader("./opengl_shaders/md2_animation_shader", shader_variables_md2);

	Texture faerie_texture("./textures/Faerie.png");
	Texture pknight_texture("./models/pknight/knight_diffuse_cheetos_1024.png");
	Texture pknight_texture_specular("./models/pknight/knight_spec_cheetos_1024.png");
	Texture bauul_texture("./textures/bauul.png");
	Texture skelett_texture("./models/skelett/skelett.png");
	Texture pknight_weapon_texture("./models/pknight/w_railgun.pcx");
	Texture goblin_texture("./models/goblin/cathos.pcx");
	Texture goblin_weapon_texture("./models/goblin/weapon.pcx");
	Texture container_texture("./textures/container2.png");

	/* position of the light */
	glm::vec3 lightPos(0.0f, 2.0f, 0.0f);

	/* replace light stuff with light class (no model) */
	Light myLight(lightPos, glm::vec3(1.0f, 1.0f, 1.0f));

	/* load MD2 model */
	md2Model md2("./models/Tris.MD2", "./textures/Abarlith_32_bpp.png");
	md2Model pknight_model("./models/pknight/tris.md2");
	md2Model faerie_model("./models/faerie.md2");
	md2Model bauul_model("./models/bauul.md2");
	md2Model skelett_model("./models/skelett/skelett.md2");
	md2Model pknight_weapon_model("./models/pknight/w_railgun.md2");
	md2Model goblin_model("./models/goblin/tris.md2");
	md2Model goblin_weapon_model("./models/goblin/weapon.md2");
	md2Model cube_model("./models/cube.md2");

	/* create a REntity, the final representation of any renderable object in the game */
	Character player;
	player.setModel(&pknight_model);
	player.setTexture(&pknight_texture, DIFFUSE);
	player.setTexture(&pknight_texture_specular, SPECULAR);

	/* NPCs */
	Character npc;
	npc.setModel(&faerie_model);
	npc.setTexture(&faerie_texture, DIFFUSE);
	Character npc2;
	npc2.setModel(&bauul_model);
	npc2.setTexture(&bauul_texture, DIFFUSE);
	Character npc3;
	npc3.setModel(&skelett_model);
	npc3.setTexture(&skelett_texture, DIFFUSE);
	Character goblin;
	goblin.setModel(&goblin_model);
	goblin.setTexture(&goblin_texture, DIFFUSE);
	Weapon goblin_weapon;
	goblin_weapon.setModel(&goblin_weapon_model);
	goblin_weapon.setTexture(&goblin_weapon_texture, DIFFUSE);
	goblin.setWeapon(goblin_weapon);
	Weapon player_weapon;
	player_weapon.setModel(&pknight_weapon_model);
	player_weapon.setTexture(&pknight_weapon_texture, DIFFUSE);
	player.setWeapon(player_weapon);
	Character cube;
	/*cube.setModel(&cube_model);
	cube.setTexture(&container_texture);*/

	/* pack the characters in a vector, maybe not the best solution for collision detection. Using something more abstract than Character (not only they can collide) would be better */
	std::vector<Character*> characters = {
		&player,
		&npc,
		&npc,
		&npc2,
		&npc3,
		&goblin
	};

	Camera gameCam;
	Camera freeCam;

	bool isClosed = false;
	//glUniform1i(glGetUniformLocation(lightMapShader.m_program, "diffuseSampler"), 0);
	//glUniform1i(glGetUniformLocation(lightMapShader.m_program, "specularSampler"), 1);
	SDL_Event event;

	/* push vertices of md2 model to gpu */
	md2.pushGPU();
	pknight_model.pushGPU();
	faerie_model.pushGPU();
	bauul_model.pushGPU();
	skelett_model.pushGPU();
	pknight_weapon_model.pushGPU();
	goblin_model.pushGPU();
	goblin_weapon_model.pushGPU();
	cube_model.pushGPU();

	// scale the bounding boxes. should probably not be here but in an init loop
	player.getBBox().scale(glm::vec3(0.15f));
	npc.getBBox().scale(glm::vec3(0.15f));
	npc2.getBBox().scale(glm::vec3(0.15f));
	npc3.getBBox().scale(glm::vec3(0.15f));
	goblin.getBBox().scale(glm::vec3(0.15f));

	/* initial setup for characters */
	npc3.setPos(glm::vec3(20, 0, 0));
	goblin.setPos(glm::vec3(20, 0, 5));
	player.getTransform().scale = glm::vec3(0.15f);
	npc.getTransform().scale = glm::vec3(0.15f);
	npc.setPos(glm::vec3(-10.0, -0.0, 0.0));
	npc2.getTransform().scale = glm::vec3(0.15f);
	npc3.getTransform().scale = glm::vec3(0.15f);
	goblin.getTransform().scale = glm::vec3(0.15f);
	npc2.getTransform().rot.y = glm::radians(270.0f);
	npc2.setPos(glm::vec3(100, 0, 0));

	// MAIN GAME LOOP ///////////////////////////////////////////////////////////////////
	while (!isClosed) {
		display.Clear(0.5f, 0.5f, 0.5f, 1.0f);

		// INPUT ////////////////////////////////////////////////////////////////////////
		// SDL2 version of input handling: 
		// SDL_PollEvent is for polling. SDL_WaitEvent will halt the program here!
		while (SDL_PollEvent(&event) != 0) 
		{
			if (event.type == SDL_QUIT)
				isClosed = true;
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				isClosed = true;
			if (event.key.keysym.scancode == SDL_SCANCODE_1)
				activeCam = 1;
			if (event.key.keysym.scancode == SDL_SCANCODE_2)
				activeCam = 2;
			if (event.type == SDL_KEYDOWN)
				keycodes[event.key.keysym.scancode] = 1;
			if (event.type == SDL_KEYUP)
				keycodes[event.key.keysym.scancode] = 0;
		}
		// ////////////////////////////////////////////////////////////////////////////////
		
		// GAME UPDATE ////////////////////////////////////////////////////////////////////

		// calculate new parameters
		freeCam.setView(freeCamPos, freeCamPos + direction, freeCamUp);
		gameCam.setView(gameCamPos + player.getTransform().pos, player.getTransform().pos, gameCamUp);	// will generate a new lookAt matrix
		if (activeCam == 1)
		{
			processCharacter(player); // update character state based on input
			globalCam = gameCam;
		}
		if (activeCam == 2)
		{
			processCamera();			  // camera movement input
			globalCam = freeCam;
		}

		md2Shader.Bind();
		// set character facing direction
		float angle = glm::angle(glm::vec3(1.0f, 0.0f, 0.0f), glm::normalize(player.getDirection()));
		if (player.getDirection().z > 0) { // for 3rd and 4th quadrants
			angle += glm::radians(180.0f);
			if (player.getDirection().x < 0.0f) {
				angle -= glm::radians(90.0f);
			}
			else if(player.getDirection().x > 0.0f){
				angle += glm::radians(90.0f);
			}
		}
		player.getTransform().rot.y = angle;

		// collision detection. check only for player and one npc for testing purposes, dammit, this part is hard!
		collisionDetect(characters);

		//// check the state of every character
		//for (auto iter = characters.begin(); iter != characters.end(); ++iter) {
		//	if ((*iter)->getHitPoints() <= 0) {
		//		(*iter)->setAnimationState(DEATH1);
		//	}
		//}

		// set animation and apply transform. TODO: pack into function!
		if (player.getState() == IDLE) {
			player.animate(1, 39, 0.009f * deltaTime);
		}
		if (player.getState() == RUN) {
			player.animate(40, 45, 0.009f * deltaTime);
			player.getTransform().pos += deltaTime * player.getVelocity() * player.getDirection();
		}
		if (player.getState() == ATTACK) {
			player.animate(46, 53, 0.015f * deltaTime);
		}
		if (player.getState() == DEATH1) {
			player.animate(178, 183, 0.009f * deltaTime);
		}

		static float lightPosX = 0.0f;
		static float lightPosY = 0.0f;
		myLight.getPosition().x = sin(lightPosX) * 5.0f;
		myLight.getPosition().y = sin(lightPosY) * 5.0f;
		lightPosX += 0.02f;
		lightPosY += 0.02f;

		// AI
		npc3.setVelocity(0.006f);
		goblin.setVelocity(0.015f);
		//computeAI(npc3, player);
		//computeAI(goblin, player);

		// END AI
		npc.animate(1, 39, 0.009f * deltaTime);
		npc2.animate(1, 39, 0.009f * deltaTime);
		npc3.animate(40, 45, 0.005f * deltaTime);
		goblin.animate(40, 45, 0.009f * deltaTime);
		//cube.animate(1, 1, 0.0f);
		// ///////////////////////////////////////////////////////////////////////////////

		// RENDER ////////////////////////////////////////////////////////////////////////
		md2Shader.Update(myLight, player, globalCam);
		player.render();
		md2Shader.Update(myLight, npc, globalCam);
		npc.render();
		md2Shader.Update(myLight, npc2, globalCam);
		npc2.render();
		md2Shader.Update(myLight, npc3, globalCam);
		npc3.render();
		md2Shader.Update(myLight, goblin, globalCam);
		goblin.render();
		//md2Shader.Update(myLight, cube, camera);
		//cube.render();
		// ///////////////////////////////////////////////////////////////////////////////

		display.SwapBuffers();

		// compute new delta time
		GLfloat currentFrame = SDL_GetTicks();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
	float FPS = 1000.0f / deltaTime;
	std::cout << FPS << std::endl;

	return 0;
}