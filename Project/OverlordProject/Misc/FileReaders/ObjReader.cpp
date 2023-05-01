#include "stdafx.h"
#include "ObjReader.h"

std::vector<VertexPosNormTexTransparency> ObjReader::ReadVertices(const std::string& meshName)
{
    std::vector<VertexPosNormTexTransparency> vertices{};

    // Get the file path to the current mesh
    std::stringstream filePath{};
    filePath << "Resources/Meshes/" << meshName << ".obj";

    // Open the file
    std::ifstream file{ filePath.str() };

    std::vector<XMFLOAT3> positions{};
    std::vector<XMFLOAT3> normals{};
    std::vector<XMFLOAT2> uvs{};

    // For each line in the file
    for (std::string line{}; std::getline(file, line); )
    {
        // If the current line is empty, continue to the next line
        if (line.empty()) continue;

        // Convert the string to a stringstream
        std::stringstream lineStream{ line };

        // Get all substrings of the line by whitespace
        std::vector<std::string> properties{ };
        std::string subString{};
        while (lineStream >> subString)
        {
            properties.emplace_back(subString);
        }

        // Get the correct data
        const std::string& typeIdentifier{ properties[0] };
        if (typeIdentifier == "v")
        {
            positions.emplace_back(XMFLOAT3
                {
                    std::stof(properties[1]),
                    std::stof(properties[2]),
                    std::stof(properties[3])
                });
        }
        else if (typeIdentifier == "vt")
        {
            uvs.emplace_back(XMFLOAT2
                {
                    std::stof(properties[1]),
                    std::stof(properties[2])
                });
        }
        else if (typeIdentifier == "vn")
        {
            normals.emplace_back(XMFLOAT3
                {
                    std::stof(properties[1]),
                    std::stof(properties[2]),
                    std::stof(properties[3])
                });
        }
        else if (typeIdentifier == "f")
        {
            // Create 4 vertices
            for (int propertyIdx{ 1 }; propertyIdx < properties.size(); ++propertyIdx)
            {
                VertexPosNormTexTransparency vertex{};

                std::string& faceData{ properties[propertyIdx] };
                constexpr char delimiter{ '/' };

                int i{};
                size_t pos = 0;
                while ((pos = faceData.find(delimiter)) != std::string::npos)
                {
                    std::string token{ faceData.substr(0, pos) };

                    const int data{ std::stoi(token) };

                    switch (i)
                    {
                    case 0:
                        vertex.Position = positions[data - 1];
                        break;
                    case 1:
                        vertex.TexCoord = uvs[data - 1];
                        break;
                    case 2:
                        vertex.Normal = normals[data - 1];
                        break;
                    }

                    faceData.erase(0, pos + sizeof(char));
                    ++i;
                }

                const int data{ std::stoi(faceData) };
                switch (i)
                {
                case 0:
                    vertex.Position = positions[data - 1];
                    break;
                case 1:
                    vertex.TexCoord = uvs[data - 1];
                    break;
                case 2:
                    vertex.Normal = normals[data - 1];
                    break;
                }

                vertices.emplace_back(vertex);
            }
        }
    }

    return vertices;
}
