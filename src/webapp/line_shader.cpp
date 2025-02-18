#include "webapp/line_shader.h"

#include <Magnum/Math/Color.h>

LineShader::LineShader() {
  Mn::GL::Shader vert{Mn::GL::Version::GLES300, Mn::GL::Shader::Type::Vertex};
  Mn::GL::Shader frag{Mn::GL::Version::GLES300, Mn::GL::Shader::Type::Fragment};

  vert.addSource(R"(
      uniform mat3 transformationProjectionMatrix;
      uniform vec4 layerColors[32];  // Assuming max 32 layers
      uniform float layerOpacities[32];
      uniform int numLayers;

      layout(location = 0) in vec2 position;
      layout(location = 1) in vec2 next;
      layout(location = 2) in int step;
      layout(location = 3) in float width;
      layout(location = 4) in int layerId;

      out vec2 circle_coords;
      flat out int fragLayerId;

      void main() {
        vec2 dir = normalize(next - position);
        vec2 normal = vec2(-dir.y, dir.x);
        vec2 offset = normal * width / 2.;

        vec2 adjusted;
        if (step == 0) {
          adjusted = position - offset;
        } else if (step == 1) {
          adjusted = next - offset;
        } else if (step == 2) {
          adjusted = next + offset;
        } else if (step == 3) {
          adjusted = position - offset;
        } else if (step == 4) {
          adjusted = next + offset;
        } else if (step == 5) {
          adjusted = position + offset;
        } else if (step == 6) {
          adjusted = next;
          adjusted += offset * 3.0 / sqrt(3.0);
          circle_coords = vec2(-sqrt(3.0), 0.0);
        } else if (step == 7) {
          adjusted = next;
          adjusted -= offset * 3.0 / sqrt(3.0);
          circle_coords = vec2(sqrt(3.0), 0.0);
        } else if (step == 8) {
          adjusted = next;
          adjusted += dir * width;
          circle_coords = vec2(0.0, 2.0);
        }

        fragLayerId = layerId;
        gl_Position = vec4(
          (transformationProjectionMatrix * vec3(adjusted, 1.0)).xy,
          float(layerId) / 32.,
          1.0
        );
      }
    )");

  frag.addSource(R"(
      precision mediump float;

      uniform vec4 layerColors[32];  // Assuming max 32 layers
      uniform float layerOpacities[32];
      uniform int numLayers;

      in vec2 circle_coords;
      flat in int fragLayerId;
      out vec4 fragColor;

      // FIXME: I do not understand how this works. It is derived from KiCAD's
      // `kicad_frag.glsl`.
      void filled_circle(vec2 coords) {
        if (dot(coords, coords) < 1.0) {
          if (fragLayerId >= 0 && fragLayerId < numLayers) {
            vec4 layerColor = layerColors[fragLayerId];
            float opacity = layerOpacities[fragLayerId];
            fragColor = vec4(layerColor.rgb, opacity);
          } else {
            fragColor = vec4(0.1, 0.1, 0.1, 1.0);
          }
        } else {
          discard;
        }
      }

      void main() {
        if (circle_coords == vec2(0.0, 0.0)) {
          if (fragLayerId >= 0 && fragLayerId < numLayers) {
            vec4 layerColor = layerColors[fragLayerId];
            float opacity = layerOpacities[fragLayerId];
            fragColor = vec4(layerColor.rgb, opacity);
          } else {
            fragColor = vec4(0.1, 0.1, 0.1, 1.0);
          }
        } else {
          filled_circle(circle_coords);
        }
      }
    )");

  CORRADE_INTERNAL_ASSERT_OUTPUT(vert.compile() && frag.compile());
  attachShaders({vert, frag});
  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  _transformationProjectionMatrixUniform =
      uniformLocation("transformationProjectionMatrix");
  _layerColorsUniform = uniformLocation("layerColors");
  _layerOpacitiesUniform = uniformLocation("layerOpacities");
  _numLayersUniform = uniformLocation("numLayers");

  setUniform(_numLayersUniform, 32);
}

auto LineShader::setTransformationProjectionMatrix(const Mn::Matrix3& matrix)
    -> LineShader& {
  setUniform(_transformationProjectionMatrixUniform, matrix);
  return *this;
}

auto LineShader::setLayerColors(const std::array<Mn::Color4, 32> colors)
    -> LineShader& {
  setUniform(_layerColorsUniform, Corrade::Containers::arrayView(colors.data(), colors.size()));
  // setUniform(_numLayersUniform, numLayers);
  return *this;
}

auto LineShader::setLayerOpacities(const std::array<float, 32> opacities)
    -> LineShader& {
  setUniform(_layerOpacitiesUniform,
             Corrade::Containers::arrayView(opacities.data(), opacities.size()));
  return *this;
}
