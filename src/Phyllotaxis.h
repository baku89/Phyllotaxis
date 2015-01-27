//
//  Phyllotaxis.h
//  reactionDiffusion
//
//  Created by Baku Hashimoto on 1/27/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxFXObject.h"

class Phyllotaxis : public ofxFXObject {
public:
    
    Phyllotaxis() {
        
        passes = 1;
        internalFormat = GL_RGB;
        
        mu = 0.001f;
        beta = 3.0f;
        
        fragmentShader = STRINGIFY(
                                   
                                   uniform sampler2DRect backbuffer;
                                   uniform sampler2DRect tex0;
                                   
                                   // parameters
                                   uniform float mu;
                                   uniform float beta;
//                                   uniform float timestep;
                                   
                                   float neighbor(vec2 pos, float x, float y) {
                                       vec2 offset = vec2(x, y);
                                       float raw = texture2DRect(backbuffer, pos + offset).r + texture2DRect(tex0, pos + offset).r;
                                       raw = clamp(raw, 0.0, 1.0);
                                       return raw - 0.5;//raw * 3.0 - 0.5;
                                   }
                                   
                                   void main() {
                            
                                       vec2 pos = gl_TexCoord[0].xy;
                                       
                                    // get neighbor pixel
                                       float a  = neighbor(pos,  0.0,  0.0);
                                       float n  = neighbor(pos,  0.0, -1.0);
                                       float n2 = neighbor(pos,  0.0, -2.0);
                                       float ne = neighbor(pos, +1.0, -1.0);
                                       float e  = neighbor(pos, +1.0,  0.0);
                                       float e2 = neighbor(pos, +2.0,  0.0);
                                       float se = neighbor(pos, +1.0, +1.0);
                                       float s  = neighbor(pos,  0.0, +1.0);
                                       float s2 = neighbor(pos,  0.0, +2.0);
                                       float sw = neighbor(pos, -1.0, +1.0);
                                       float w  = neighbor(pos, -1.0,  0.0);
                                       float w2 = neighbor(pos, -2.0,  0.0);
                                       float nw = neighbor(pos, -1.0, -1.0);
                                       
                                       float timestep = 0.0001;
                                       float h = 0.55; // grid spacing
                                       
                                       float laplacian = (n + e + s + w - 4.0 * a) / (h * h);
                                       float bilaplacian = (n2 + e2 + s2 + w2 + 2.0 * (ne + se + sw + nw)
                                                            - 8.0 * (n + e + s + w) + 20.0 * a) / (h * h * h * h);
                                       float x_gradient = (w - e) / (2.0 * h);
                                       float y_gradient = (s - n) / (2.0 * h);
                                       float gradient_mag_squared_a = pow( x_gradient, 2.0 ) + pow( y_gradient, 2.0);
                                       
                                       float delta_a = mu * a - (bilaplacian + 2.0*laplacian + a) - (beta / 3.0) * (gradient_mag_squared_a + 2.0 * a * laplacian) - a * a * a;
                                       
                                       float result = a + timestep * delta_a;
                                       
                                       gl_FragColor = vec4(result +
                                                           0.5, 0.0, 0.0, 1.0);
                                       //gl_FragColor = vec4(a, 0.0, 0.0, 1.0);
                                   }
        
        );
        
    };
    
    void update() {
        
        pingPong.dst->begin();
        ofClear(0, 0, 0, 255);
        
        shader.begin();
        shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0);
        shader.setUniformTexture("tex0", textures[0].getTextureReference(), 1);
//        shader.setUniform1f("timestep", (float)0.0001);
//        shader.setUniform1f("h", (float)0.055);
        shader.setUniform1f("mu", (float)mu);
        shader.setUniform1f("beta", (float)beta);
        renderFrame();
        shader.end();
        pingPong.dst->end();
        
        pingPong.swap();
    
    };
    
    float mu, beta;
};