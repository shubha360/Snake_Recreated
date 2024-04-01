#include "../include/Consumable.h"

std::random_device Consumable::seed_;
std::mt19937 Consumable::randomEngine_(seed_());

void Consumable::loadTexture(const std::string& path, Evolve::TextureData& texture) {
	Evolve::ImageLoader::LoadTextureFromImage(path, texture, 4);
	Evolve::ImageLoader::BufferTextureData(texture);
	Evolve::ImageLoader::FreeTexture(texture);
}
