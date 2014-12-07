//
//  CompoShaders.h
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_SHADERS_SOURCES_H
#define COMPO_SHADERS_SOURCES_H

#define COMPO_SHADER_SRC(x) #x

// Vertex Shaders Sources
static const std::string VS_ScreenAlignedQuad = COMPO_SHADER_SRC(
  attribute vec3 position;
  attribute vec2 texcoord;
  varying   vec2 vTexCoord;
  const vec2 madd = vec2(0.5, 0.5);
  uniform mat4 mvpMatrix;
  void main(void) {
    gl_Position = mvpMatrix * vec4(position.xy, 0.0, 1.0);
    vTexCoord =  vec2(position.x, -position.y) * madd + madd;
  }
);

static const std::string PS_GreyQuad = COMPO_SHADER_SRC(
  precision mediump float;
  void main() {
    gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0);
  }
);

static const std::string PS_EffectNormal = COMPO_SHADER_SRC(
  precision lowp float;
  varying highp vec2 vTexCoord;
  uniform sampler2D tex0;
  void main() {
    gl_FragColor = texture2D(tex0, vTexCoord);
  }
);


static const std::string PS_EffectGreyScale = COMPO_SHADER_SRC(
  precision mediump float;
  varying highp vec2 vTexCoord;
  uniform sampler2D tex0;
  void main() {
    vec4 color = texture2D(tex0, vTexCoord);
    float avg = (color.r + color.g + color.b) / 3.0;
    gl_FragColor = vec4(avg, avg, avg, 1.0);
  }
);

static const std::string PS_EffectBleach =
  "precision mediump float;"
  "varying highp vec2 vTexCoord;"
  "uniform sampler2D tex0;"
  ""
  "const vec3 luminance = vec3(0.3086, 0.6094, 0.0820);"
  "const vec2 midway = vec2(0.5, 0.5); const vec3 one = vec3(1.0);"
  "const vec3 two = vec3(2.0);"
  "const mat3 saturation = mat3(1.13828, -0.06172, -0.06172,"
  "  -0.12188, 1.07812, -0.12188, -0.0164, -0.0164, 1.1836);"
  "void main() {"
  "	 vec3 color = texture2D(tex0, vTexCoord).rgb;"
  "	 color *= smoothstep(1.4, 0.2, distance(vTexCoord, midway));"
  "	 color = saturation * color; float luminance = dot(color, luminance);"
  "	 vec3 luma = vec3(luminance); float mixamount = clamp((luminance - 0.45) * 10.0, 0.0, 1.0);"
  "	 vec3 branch1 = two * color * luma;"
  "	 vec3 branch2 = one - (two * (one - color) * (one - luma));"
  "	 color = mix(color, mix(branch1, branch2, vec3(mixamount)), 1.0);"
  "	 gl_FragColor = vec4(color, 1.0);"
  "}";

static const std::string PS_EffectSepia =
"precision lowp float;"
"varying highp vec2 vTexCoord;                             "
"uniform sampler2D tex0;                             "
""
"vec3 overlay(vec3 src, vec3 dst) {                  "
"  return vec3((dst.x <= 0.5) ? (2.0 * src.x * dst.x) : "
"	 (1.0 - 2.0 * (1.0 - dst.x) * (1.0 - src.x)),       "
"	 (dst.y <= 0.5) ? (2.0 * src.y * dst.y) :           "
"	 (1.0 - 2.0 * (1.0 - dst.y) * (1.0 - src.y)),       "
"	 (dst.z <= 0.5) ? (2.0 * src.z * dst.z) :           "
"	 (1.0 - 2.0 * (1.0 - dst.z) * (1.0 - src.z)));      "
"} "
"void main(void) { "
"  const vec3 sepia = vec3(112.0 / 255.0, 66.0 / 255.0, 20.0 / 255.0);"
"  vec4 color = texture2D(tex0, vTexCoord);"
"  float avg = (color.r + color.g + color.b) / 3.0;"
"  vec3 grayscale = vec3(avg);"
"  gl_FragColor = vec4(overlay(sepia, grayscale), 1.0);"
"} ";

#endif
