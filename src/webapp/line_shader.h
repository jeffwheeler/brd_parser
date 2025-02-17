#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Matrix3.h>

class LineShader : public Magnum::GL::AbstractShaderProgram {
 public:
  using Position = Magnum::GL::Attribute<0, Magnum::Vector2>;
  using Width = Magnum::GL::Attribute<1, Magnum::Float>;
  using Color = Magnum::GL::Attribute<2, Magnum::Color3>;

  enum class Uniform : Magnum::Int { TransformationProjectionMatrix = 0 };

  LineShader() {
    Magnum::GL::Shader vert{Magnum::GL::Version::GLES300,
                            Magnum::GL::Shader::Type::Vertex};
    Magnum::GL::Shader frag{Magnum::GL::Version::GLES300,
                            Magnum::GL::Shader::Type::Fragment};

    vert.addSource(R"(
      uniform mat3 transformationProjectionMatrix;

      layout(location = 0) in vec2 position;
      layout(location = 1) in float width;
      layout(location = 2) in vec3 color;

      out vec3 fragmentColor;

      void main() {
        fragmentColor = color;
        vec2 adjusted = vec2(position.x, position.y + width);
        gl_Position = vec4(transformationProjectionMatrix * vec3(adjusted, 1.0), 1.0);
      }
    )");

    frag.addSource(R"(
      in mediump vec3 fragmentColor;
      out mediump vec4 fragColor;

      void main() {
        fragColor = vec4(fragmentColor, 1.0);
      }
    )");

    CORRADE_INTERNAL_ASSERT_OUTPUT(vert.compile() && frag.compile());
    attachShaders({vert, frag});
    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

    _transformationProjectionMatrixUniform =
        uniformLocation("transformationProjectionMatrix");
  }

  auto setTransformationProjectionMatrix(const Magnum::Matrix3& matrix)
      -> LineShader& {
    setUniform(_transformationProjectionMatrixUniform, matrix);
    return *this;
  }

 private:
  Magnum::Int _transformationProjectionMatrixUniform;
};
