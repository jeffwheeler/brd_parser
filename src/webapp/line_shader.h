#pragma once

#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>
#include <Magnum/Math/Matrix3.h>

using namespace Magnum;

class LineShader : public Magnum::GL::AbstractShaderProgram {
 public:
  using Position = Magnum::GL::Attribute<0, Magnum::Vector2>;
  using Color = Magnum::GL::Attribute<1, Magnum::Color3>;

  enum class Uniform : Int { TransformationProjectionMatrix = 0 };

  LineShader() {
    GL::Shader vert{GL::Version::GLES300, GL::Shader::Type::Vertex};
    GL::Shader frag{GL::Version::GLES300, GL::Shader::Type::Fragment};

    vert.addSource(R"(
        uniform mat3 transformationProjectionMatrix;

        layout(location = 0) in vec2 position;
        layout(location = 1) in vec3 color;

        out vec3 fragmentColor;

        void main() {
            fragmentColor = color;
            gl_Position = vec4(transformationProjectionMatrix * vec3(position, 1.0), 1.0);
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
  Int _transformationProjectionMatrixUniform;
};
