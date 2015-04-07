options = -Iinclude -lGL -lGLU -lSDL2

all: main.exe

main.exe: main.o Shader.o ShaderProgram.o stb_image.o Debug.o Time.o VBO.o VAO.o Image.o Texture.o FileLoader.o FrameBuffer.o Mesh.o GBuffer.o Light.o StateManager.o Material.o GameObject.o Scene.o RenderTarget.o CubeMap.o SkyBox.o Component.o Camera.o Quaternion.o Transform.o
	g++ -std=c++11 *.o -o main.exe $(options)

main.o: src/main.cpp
	g++ -std=c++11 -c src/main.cpp $(options)

Shader.o: src/Shader.cpp
	g++ -std=c++11 -c src/Shader.cpp $(options)

ShaderProgram.o: src/ShaderProgram.cpp
	g++ -std=c++11 -c src/ShaderProgram.cpp $(options)

Debug.o: src/Debug.cpp
	g++ -std=c++11 -c src/Debug.cpp $(options)

Time.o: src/Time.cpp
	g++ -std=c++11 -c src/Time.cpp $(options)

VBO.o: src/VBO.cpp
	g++ -std=c++11 -c src/VBO.cpp $(options)

VAO.o: src/VAO.cpp
	g++ -std=c++11 -c src/VAO.cpp $(options)

Image.o: src/Image.cpp
	g++ -std=c++11 -c src/Image.cpp $(options)

Texture.o: src/Texture.cpp
	g++ -std=c++11 -c src/Texture.cpp $(options)

FileLoader.o: src/FileLoader.cpp
	g++ -std=c++11 -c src/FileLoader.cpp $(options)

FrameBuffer.o: src/FrameBuffer.cpp
	g++ -std=c++11 -c src/FrameBuffer.cpp $(options)

stb_image.o: src/stb_image.cpp
	g++ -std=c++11 -c src/stb_image.cpp $(options)

Mesh.o: src/Mesh.cpp
	g++ -std=c++11 -c src/Mesh.cpp $(options)

GBuffer.o: src/GBuffer.cpp
	g++ -std=c++11 -c src/GBuffer.cpp $(options)

Light.o: src/Light.cpp
	g++ -std=c++11 -c src/Light.cpp $(options)

StateManager.o: src/StateManager.cpp
	g++ -std=c++11 -c src/StateManager.cpp $(options)

Material.o: src/Material.cpp
	g++ -std=c++11 -c src/Material.cpp $(options)

GameObject.o: src/GameObject.cpp
	g++ -std=c++11 -c src/GameObject.cpp $(options)

Scene.o: src/Scene.cpp
	g++ -std=c++11 -c src/Scene.cpp $(options)

RenderTarget.o: src/RenderTarget.cpp
	g++ -std=c++11 -c src/RenderTarget.cpp $(options)

CubeMap.o: src/CubeMap.cpp
	g++ -std=c++11 -c src/CubeMap.cpp $(options)

SkyBox.o: src/SkyBox.cpp
	g++ -std=c++11 -c src/SkyBox.cpp $(options)

Component.o: src/Component.cpp
	g++ -std=c++11 -c src/Component.cpp $(options)

Camera.o: src/Camera.cpp
	g++ -std=c++11 -c src/Camera.cpp $(options)

Quaternion.o: src/Quaternion.cpp
	g++ -std=c++11 -c src/Quaternion.cpp $(options)

Transform.o: src/Transform.cpp
	g++ -std=c++11 -c src/Transform.cpp $(options)


clean:
	rm -rf *.exe *.o *~
