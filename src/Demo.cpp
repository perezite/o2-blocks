#include "Window.h"
#include "DrawBatch.h"
#include "Triangle.h"
#include "Quad.h"
#include "Input.h"
#include "Stopwatch.h"
#include "TextureSheet.h"
#include "Sprite.h"
#include "Math.h"
#include <cstddef>
#include <vector>
#include <map>

void init0(std::vector<sb::Sprite>& stones, sb::Texture& stonesTex) {
	sb::Vector2i size(128, 128);
	sb::Vector2f scale(0.2f, 0.2f);

	stones[0].setTexture(stonesTex, sb::IntRect(0 * size.x, 0 * size.y, size.x, size.y));
	stones[1].setTexture(stonesTex, sb::IntRect(0 * size.x, 1 * size.y, size.x, size.y));
	stones[2].setTexture(stonesTex, sb::IntRect(1 * size.x, 1 * size.y, size.x, size.y));
	stones[3].setTexture(stonesTex, sb::IntRect(2 * size.x, 1 * size.y, size.x, size.y));
	stones[4].setTexture(stonesTex, sb::IntRect(0 * size.x, 2 * size.y, size.x, size.y));
	stones[5].setTexture(stonesTex, sb::IntRect(1 * size.x, 2 * size.y, size.x, size.y));
	stones[6].setTexture(stonesTex, sb::IntRect(2 * size.x, 2 * size.y, size.x, size.y));

	for (std::size_t i = 0; i < stones.size(); i++) {
		sb::Vector2i gridPos(i % 3, i / 3);
		float x = (gridPos.x - 1) * scale.x * 1.1f;
		float y = (gridPos.y - 1) * scale.y * 1.1f;
		stones[i].setPosition(x, y);
		stones[i].setScale(scale);
	}
}

void draw0(sb::DrawBatch& batch, std::vector<sb::Sprite>& stones) {
	for (std::size_t i = 0; i < stones.size(); i++) {
		batch.draw(stones[i]);
	}
}

void demo0() {
	float aspect = 3 / 2.0f;
	sb::Window window(400, int(400 * aspect));
	sb::DrawBatch batch(512);
	sb::Texture stoneTex;
	std::vector<sb::Sprite> stones(7);

	stoneTex.loadFromAsset("Textures/Stones.png");
	init0(stones, stoneTex);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear();
		draw0(batch, stones);
		window.draw(batch);
		window.display();
	}
}

void init1(sb::Mesh& background, sb::Camera& camera) {
	sb::Vector2f extent(camera.getWidth() * 0.5f, camera.getWidth() * camera.getInverseAspectRatio() * 0.5f);
	sb::Color bottomColor = sb::Color(252.0f / 255.0f, 182.0f / 255.0f, 159.0f / 255.0f, 1);
	sb::Color topColor = sb::Color(255.0f / 255.0f, 236.0f / 255.0f, 210.0f / 255.0f, 1);
	background[0] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), bottomColor);
	background[1] = sb::Vertex(sb::Vector2f(+extent.x, -extent.y), bottomColor);
	background[2] = sb::Vertex(sb::Vector2f(-extent.x, +extent.y), topColor);
	background[3] = sb::Vertex(sb::Vector2f(+extent.x, +extent.y), topColor);
}

void demo1() {
	float aspect = 3 / 2.0f;
	sb::Window window(400, int(400 * aspect));
	sb::Mesh background(4, sb::PrimitiveType::TriangleStrip);

	init1(background, window.getCamera());

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear();
		window.draw(background.getVertices(), sb::PrimitiveType::TriangleStrip);
		window.display();
	}
}

void initCamera(sb::Camera& camera, float minInverseAspectRatio) {
	float minHeight = minInverseAspectRatio;
	float height = camera.getWidth() * camera.getInverseAspectRatio();
	if (height < minHeight) {
		float ratio = minHeight / height;
		camera.setWidth(camera.getWidth() * ratio);
	}
}

void demo2() {
	float inverseAspect = 2 / 4.0f;
	sb::Window window(400, int(400 * inverseAspect));
	sb::Texture coordinatesTex;
	sb::Sprite coordinates;
	sb::Quad reference;

	initCamera(window.getCamera(), 3 / 2.0f);
	coordinatesTex.loadFromAsset("Textures/CoordinateSystem.png");
	coordinates.setTexture(coordinatesTex);
	reference.setScale(0.1f, 1.45f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(coordinates);
		window.draw(reference);

		window.display();
	}
}

inline float getCameraHeight(sb::Camera& camera) {
	return camera.getWidth() * camera.getInverseAspectRatio();
}

inline sb::Color colorFromRgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
	return sb::Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

class Border : public sb::Drawable, public sb::Transformable {
private:
	sb::Mesh _mesh;
	float _height;

public:
	Border(float height, float thickness)
		: _mesh(10, sb::PrimitiveType::TriangleStrip), _height(height) {
		sb::Vector2f extent(0.5f, height / 2);
		sb::Color innerColor = colorFromRgba(143, 211, 244);
		sb::Color outerColor = colorFromRgba(132, 250, 176);

		_mesh[0] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
		_mesh[1] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
		_mesh[2] = sb::Vertex(sb::Vector2f(+extent.x, -extent.y), outerColor);
		_mesh[3] = sb::Vertex(sb::Vector2f(+extent.x - thickness, -extent.y + thickness), innerColor);
		_mesh[4] = sb::Vertex(sb::Vector2f(+extent.x, +extent.y), outerColor);
		_mesh[5] = sb::Vertex(sb::Vector2f(+extent.x - thickness, +extent.y - thickness), innerColor);
		_mesh[6] = sb::Vertex(sb::Vector2f(-extent.x, +extent.y), outerColor);
		_mesh[7] = sb::Vertex(sb::Vector2f(-extent.x + thickness, +extent.y - thickness), innerColor);
		_mesh[8] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
		_mesh[9] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
	}

	inline float getHeight() const { return _height; }

	void attachToTop(sb::Camera& targetCamera) {
		float middle = getCameraHeight(targetCamera) / 2 - getHeight() / 2;
		translate(0, middle);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
	}
};

void demo3() {
	float aspect = 3 / 2.0f;
	sb::Window window(400, int(400 * aspect));
	sb::Mesh background(4, sb::PrimitiveType::TriangleStrip);
	Border border(0.9f * getCameraHeight(window.getCamera()), 0.01f);

	init1(background, window.getCamera());
	border.attachToTop(window.getCamera());

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear();
		window.draw(background.getVertices(), background.getPrimitiveType());
		window.draw(border);
		window.display();
	}
}

class Grid : public sb::Drawable {
private:
	sb::Mesh _mesh;

public:
	Grid(std::size_t numColums, std::size_t numRows, float width, float height, float thickness, const sb::Color& color = sb::Color(1, 0, 0, 1))
		: _mesh((numRows + numColums + 2) * 6, sb::PrimitiveType::TriangleStrip)
	{
		addVerticalLines(numColums + 1, numRows + 1, width, height, thickness, color);
		addHorizontalLines(numRows + 1, width, height, thickness, color);
	}

	void addVerticalLines(std::size_t numColumns, std::size_t indexOffset, float width, float height, float thickness, const sb::Color& color) {
		float delta = width / (numColumns - 1);
		for (std::size_t i = 0; i < numColumns; i++)
			addVerticalLine(i + indexOffset, i * delta - width / 2, height, thickness, color);
	}

	void addVerticalLine(std::size_t index, float horizontalPosition, float height, float thickness, const sb::Color& color) {
		_mesh[index * 6 + 0] = sb::Vertex(sb::Vector2f(horizontalPosition + thickness / 2, -height / 2), color);
		_mesh[index * 6 + 1] = sb::Vertex(sb::Vector2f(horizontalPosition + thickness / 2, -height / 2), color);
		_mesh[index * 6 + 2] = sb::Vertex(sb::Vector2f(horizontalPosition - thickness / 2, -height / 2), color);
		_mesh[index * 6 + 3] = sb::Vertex(sb::Vector2f(horizontalPosition + thickness / 2, +height / 2), color);
		_mesh[index * 6 + 4] = sb::Vertex(sb::Vector2f(horizontalPosition - thickness / 2, +height / 2), color);
		_mesh[index * 6 + 5] = sb::Vertex(sb::Vector2f(horizontalPosition - thickness / 2, +height / 2), color);
	}

	void addHorizontalLines(std::size_t numRows, float width, float height, float thickness, const sb::Color& color) {
		float delta = height / (numRows - 1);
		for (std::size_t i = 0; i < numRows; i++)
			addHorizontalLine(i, i * delta - height / 2, width, thickness, color);
	}

	void addHorizontalLine(std::size_t index, float verticalPosition, float width, float thickness, const sb::Color& color) {
		_mesh[index * 6 + 0] = sb::Vertex(sb::Vector2f(-width / 2, verticalPosition - thickness / 2), color);
		_mesh[index * 6 + 1] = sb::Vertex(sb::Vector2f(-width / 2, verticalPosition - thickness / 2), color);
		_mesh[index * 6 + 2] = sb::Vertex(sb::Vector2f(-width / 2, verticalPosition + thickness / 2), color);
		_mesh[index * 6 + 3] = sb::Vertex(sb::Vector2f(+width / 2, verticalPosition - thickness / 2), color);
		_mesh[index * 6 + 4] = sb::Vertex(sb::Vector2f(+width / 2, verticalPosition + thickness / 2), color);
		_mesh[index * 6 + 5] = sb::Vertex(sb::Vector2f(+width / 2, verticalPosition + thickness / 2), color);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		target.draw(_mesh.getVertices(), _mesh.getPrimitiveType());
	}
};

void demo4() {
	float aspect = 3 / 2.0f;
	sb::Window window(400, int(400 * aspect));
	Grid grid(10, 18, 1, getCameraHeight(window.getCamera()), 0.005f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(grid);
		window.display();
	}
}

class Light {
	sb::Vector2f _direction;

public: 
	Light(const sb::Vector2f& direction = sb::Vector2f(0, -1)) : _direction(direction)
	{ }

	inline void setDirection(const sb::Vector2f& direction) { _direction = direction; }

	inline const sb::Vector2f& getDirection() const { return _direction; }
};

class Block : public sb::Drawable, public sb::Transformable {
	sb::Sprite _sprite;
	const Light* _light;

protected:
	static sb::Texture& getSheet() {
		static sb::Texture texture("Textures/Blocks.png");
		return texture;
	}

	inline const sb::IntRect getTextureArea(std::size_t row, std::size_t col) {
		static sb::Vector2i blockSize(128, 128);
		return sb::IntRect(col * blockSize.x, row * blockSize.y, blockSize.x, blockSize.y);
	}

	void setTexture(char type) {
		if (type == 'i')
			_sprite.setTexture(getSheet(), getTextureArea(2, 1));
		else if (type == 'j')
			_sprite.setTexture(getSheet(), getTextureArea(2, 2));
		else if (type == 'm')
			_sprite.setTexture(getSheet(), getTextureArea(2, 1));
		else
			SB_ERROR("Invalid Tetromino type " << type);
	}

	void updateLighting() {
		if (_light) {
			sb::Vector2f up(0, 1);
			float angle = sb::angle(-_light->getDirection(), up) + 45 * sb::ToRadian;
			angle = angle < 0 ? angle + 2 * sb::Pi : angle;
			int steps = int(angle / (90 * sb::ToRadian));
			 _sprite.setRotation(-steps * 90 * sb::ToRadian);
		}
	}

public:
	Block(char type = 'i') : _light(NULL) 
	{
		setType(type);
	}

	void setType(char type) {
		type = tolower(type);
		setTexture(type);
	}

	inline void setLight(const Light& light) { _light = &light; }

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		updateLighting();
		target.draw(_sprite, states);
	}
};

void init5(std::vector<Block>& blocks) {
	static std::vector<char> types = { 'i', 'j', 'm' };
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i].setType(types[rand() % types.size()]);
		blocks[i].setPosition(sb::random2D(-0.5f, 0.5f));
		blocks[i].setScale(sb::random(0.1f, 0.2f));
	}
}

void draw5(sb::DrawBatch& batch, std::vector<Block>& blocks) {
	for (size_t i = 0; i < blocks.size(); i++)
		batch.draw(blocks[i]);
}

void demo5() {
	float aspect = 3 / 2.0f;
	sb::DrawBatch batch(512);
	sb::Window window(400, int(400 * aspect));
	std::vector<Block> blocks(20);

	init5(blocks);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		draw5(batch, blocks);
		window.draw(batch);
		window.display();
	}
}

inline int getWindowHeight(int width) {
	float aspect = 3 / 2.0f;
	return int(width * aspect);
}

class Tetromino : public sb::Drawable, public sb::Transformable {
	std::vector<Block> _blocks;
	std::vector<sb::Vector2i> _blockPositions;

protected:
	void clear() {
		_blocks.clear();
		_blockPositions.clear();	
	}

	sb::Vector2i getBlockDimensions() {
		sb::Vector2i minimum(_blockPositions[0].x, _blockPositions[0].y);
		sb::Vector2i maximum(minimum);

		for (size_t i = 0; i < _blockPositions.size(); i++) {
			sb::Vector2i& pos = _blockPositions[i];
			if (pos.x < minimum.x)
				minimum.x = pos.x;
			if (pos.x > maximum.x)
				maximum.x = pos.x;
			if (pos.y < minimum.y)
				minimum.y = pos.y;
			if (pos.y > maximum.y)
				maximum.y = pos.y;
		}

		return maximum - minimum + sb::Vector2i(1, 1);
	}

	void createBlocks(const std::vector<sb::Vector2i>& positions, char type) {
		sb::Vector2i blockDimensions = getBlockDimensions();
		for (size_t i = 0; i < positions.size(); i++) {
			Block block;
			block.setPosition(positions[i].x / float(blockDimensions.x), positions[i].y / float(blockDimensions.y));
			block.setScale(1.0f / blockDimensions.x, 1.0f / blockDimensions.y);
			block.setType(type);
			_blocks.push_back(block);
		}
	}

	void createBlocks(char type) {
		if (type == 'i')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(2, 0) };
		else if (type == 'j')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(0, -1), sb::Vector2i(1, -1) };
		else if (type == 'm')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0) };

		createBlocks(_blockPositions, type);
	}

	void updateScale() {
		sb::Vector2i blockDimensions = getBlockDimensions();
		setScale(float(blockDimensions.x), float(blockDimensions.y));
	}

public:
	Tetromino(char type = 'i') {
		setType(type);
	}

	void setType(char type) {
		clear();
		createBlocks(tolower(type));
		updateScale();
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		for (size_t i = 0; i < _blocks.size(); i++)
			target.draw(_blocks[i], states);
	}
};

void init6(std::vector<Tetromino>& tetrominos) {
	srand(385949385);
	static std::vector<char> types = { 'i', 'j' };
	for (size_t i = 0; i < tetrominos.size(); i++) {
		tetrominos[i].setType(types[rand() % types.size()]);
		tetrominos[i].setPosition(sb::random2D(-0.5f, 0.5f));
		tetrominos[i].setScale(sb::random(0.01f, 0.1f) * tetrominos[i].getScale());
	}
}

void draw6(sb::DrawBatch& batch, std::vector<Tetromino>& tetrominos) {
	for (size_t i = 0; i < tetrominos.size(); i++)
		batch.draw(tetrominos[i]);
}

void demo6() {
	sb::DrawBatch batch(512);
	sb::Window window(400, getWindowHeight(400));
	std::vector<Tetromino> tetrominos(10);

	init6(tetrominos);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		draw6(batch, tetrominos);
		window.draw(batch);
		window.display();
	}
}

namespace {
	float getSeconds() {
		static sb::Stopwatch sw;
		return sw.getElapsedSeconds();
	}
}

void setSpotlightPosition(sb::Triangle& spotlight, Light& light, const sb::Vector2f& position) {
	spotlight.setPosition(position);
	light.setDirection(-spotlight.getPosition());
}

void update7(sb::Triangle& spotlight, Light& light) {
	float t = getSeconds();
	sb::Vector2f spotlightPosition(0.2f * cos(-t), 0.2f * sin(-t));
	setSpotlightPosition(spotlight, light, spotlightPosition);
}

void demo7() {
	sb::Window window(400, getWindowHeight(400));
	Block block('i');
	Light light;
	sb::Triangle spotlight;

	block.setScale(0.2f, 0.2f);
	block.setLight(light);
	spotlight.setScale(0.05f, 0.05f);


	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		 update7(spotlight, light);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.draw(spotlight);
		window.display();
	}
}

void demo() {
	demo7();

	//demo6();

	//demo5();

	// demo4();

	//demo3();

	//demo2();

	//demo1();

	// demo0();
}