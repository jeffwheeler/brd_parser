#include "webapp/line_shader.h"

LineShader::LineShader() {
  Mn::GL::Shader vert{Mn::GL::Version::GLES300, Mn::GL::Shader::Type::Vertex};
  Mn::GL::Shader frag{Mn::GL::Version::GLES300, Mn::GL::Shader::Type::Fragment};

  vert.addSource(R"(
      uniform mat3 transformationProjectionMatrix;

      layout(location = 0) in vec2 position;
      layout(location = 1) in vec2 next;
      layout(location = 2) in int step;
      layout(location = 3) in float width;
      layout(location = 4) in vec4 color;

      out vec4 fragmentColor;

      void main() {
        fragmentColor = color;

        vec2 dir = normalize(next - position);
        vec2 normal = vec2(-dir.y, dir.x);
        vec2 offset = normal * width;

        vec2 adjusted;
        if (step == 0) {
          adjusted.x = position.x;
          adjusted.y = position.y;
          adjusted -= offset;
        } else if (step == 1) {
          adjusted.x = next.x;
          adjusted.y = next.y;
          adjusted -= offset;
        } else if (step == 2) {
          adjusted.x = next.x;
          adjusted.y = next.y;
          adjusted += offset;
        } else if (step == 3) {
          adjusted.x = position.x;
          adjusted.y = position.y;
          adjusted -= offset;
        } else if (step == 4) {
          adjusted.x = next.x;
          adjusted.y = next.y;
          adjusted += offset;
        } else if (step == 5) {
          adjusted.x = position.x;
          adjusted.y = position.y;
          adjusted += offset;
        }
        gl_Position = vec4(transformationProjectionMatrix * vec3(adjusted, 1.0), 1.0);
      }
    )");

  frag.addSource(R"(
      in mediump vec4 fragmentColor;
      out mediump vec4 fragColor;

      void main() {
        fragColor = fragmentColor;
      }
    )");

  CORRADE_INTERNAL_ASSERT_OUTPUT(vert.compile() && frag.compile());
  attachShaders({vert, frag});
  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  _transformationProjectionMatrixUniform =
      uniformLocation("transformationProjectionMatrix");
}

auto LineShader::setTransformationProjectionMatrix(const Mn::Matrix3& matrix)
    -> LineShader& {
  setUniform(_transformationProjectionMatrixUniform, matrix);
  return *this;
}
