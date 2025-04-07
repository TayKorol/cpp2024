//
// Created by user on 01.04.2025.
//
#pragma once

#include <string>
#include <memory>
#include <map>
#include <random>
#include "SFML/Graphics.hpp"


namespace Textures {
    enum class ID {
        Texture_Grass,
        Texture_Hill,
        Texture_Forest,
        Texture_Stone,
        Texture_Sand,
        Texture_Snow,
        Texture_Water,
        Texture_Hide,
        Texture_Soup,
        Texture_EvilGuy
    };

    static ID getTileTexture(std::mt19937 &gen) {
        static const std::vector<ID> textures = {
                ID::Texture_Grass,
                ID::Texture_Hill,
                ID::Texture_Forest,
                ID::Texture_Stone,
                ID::Texture_Sand,
                ID::Texture_Snow,
                ID::Texture_Water
        };

        std::uniform_int_distribution<size_t> dist(0, textures.size() - 1);

        return textures[dist(gen)];
    }
}
namespace Fonts {
    enum class ID {
        Arial
    };
}


template<typename T>
concept HasLoadFromFile = requires(T &t, const std::string &path) {
    { t.loadFromFile(path) } -> std::convertible_to<bool>;
};

template<typename T>
concept HasOpenFromFile = requires(T &t, const std::string &path) {
    { t.openFromFile(path) } -> std::convertible_to<bool>;
};


template<typename Resource, typename ID>
class ResourceHolder {
private:
    std::map<ID, std::unique_ptr<Resource>> resourceMap;
public:
    ResourceHolder(std::map<ID, std::string> resourcePaths) {
        for (auto pair: resourcePaths) {
            loadFromPath(pair.first, pair.second);
        }
    }

    void loadFromPath(ID id, const std::string &path) {
        std::unique_ptr<Resource> resource(new Resource());


        sf::Texture texture;
        sf::Font font;

        if (!loaded(path, resource)) {
            throw std::runtime_error("resourceMap::load - Failed to load " + path);
        }
        assert(resourceMap.find(id) == resourceMap.end());
        resourceMap.insert(
                std::make_pair(id, std::move(resource)));
    }

    bool loaded(const std::string &path,
                const std::unique_ptr<Resource> &resource) const requires HasLoadFromFile<Resource> {
        return resource->loadFromFile(path);
    }

    bool loaded(const std::string &path,
                const std::unique_ptr<Resource> &resource) const requires HasOpenFromFile<Resource> {
        return resource->openFromFile(path);
    }


    [[nodiscard]] const Resource &get(ID id) const {
        auto item = resourceMap.find(id);
        return *item->second;
    }

    Resource &get(ID id) {
        auto item = resourceMap.find(id);
        return *item->second;
    }
};

