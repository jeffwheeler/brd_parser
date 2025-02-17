#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Matrix3.h>

namespace Mn = Magnum;

class LineShader : public Mn::GL::AbstractShaderProgram {
 public:
  using Position = Mn::GL::Attribute<0, Mn::Vector2>;
  using Next = Mn::GL::Attribute<1, Mn::Vector2>;
  using Step = Mn::GL::Attribute<2, Mn::Int>;
  using Width = Mn::GL::Attribute<3, Mn::Float>;
  using Color = Mn::GL::Attribute<4, Mn::Color4>;

  enum class Uniform : Mn::Int { TransformationProjectionMatrix = 0 };

  LineShader();
  auto setTransformationProjectionMatrix(const Mn::Matrix3& matrix)
      -> LineShader&;

 private:
  Mn::Int _transformationProjectionMatrixUniform;
};
