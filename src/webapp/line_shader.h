#pragma once

#include <array>

#include <Corrade/Containers/ArrayView.h>
#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Matrix3.h>

namespace Cr = Corrade;
namespace Mn = Magnum;

class LineShader : public Mn::GL::AbstractShaderProgram {
 public:
  using Position = Mn::GL::Attribute<0, Mn::Vector2>;
  using Next = Mn::GL::Attribute<1, Mn::Vector2>;
  using Step = Mn::GL::Attribute<2, Mn::Int>;
  using Width = Mn::GL::Attribute<3, Mn::Float>;
  using LayerId = Mn::GL::Attribute<4, Mn::Int>;

  enum class Uniform : Mn::Int {
    TransformationProjectionMatrix = 0,
    LayerColors = 1,
    LayerOpacities = 2,
    NumLayers = 3
  };

  explicit LineShader();

  auto setTransformationProjectionMatrix(const Mn::Matrix3& matrix)
      -> LineShader&;
  auto setLayerColors(std::array<Mn::Color4, 32> colors) -> LineShader&;
  auto setLayerOpacities(std::array<float, 32> opacities) -> LineShader&;

 private:
  Mn::Int _transformationProjectionMatrixUniform;
  Mn::Int _layerColorsUniform;
  Mn::Int _layerOpacitiesUniform;
  Mn::Int _numLayersUniform;
};
