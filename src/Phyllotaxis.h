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
#include "ofxOpenCv.h"

class Phyllotaxis : public ofxFXObject {
public:
    
    Phyllotaxis() {
        
        passes = 2;
        internalFormat = GL_RGB32F;
        
        timestep = 0.0008f;
        h = 0.4f;
        mu = 0.001f;
        beta = 3.0f;
        
        isInit = 0;
        
        fragmentShader = STRINGIFY(
                                   uniform sampler2DRect backbuffer;
                                   uniform sampler2DRect tex0;          // initial pattern
                                   uniform sampler2DRect tex1;          // mask
                                   
                                   uniform float timestep;
                                   uniform float h;
                                   uniform float mu;
                                   uniform float beta;
                                   uniform int   isInit;
                                   
                                   float neighbor(vec2 pos, float x, float y) {
                                       
                                       vec2 offset = pos + vec2(x, y);
                                       
                                       float raw = texture2DRect(backbuffer, offset).r;
                                       
                                       if (texture2DRect(tex0, offset).g < 0.5) {
                                           raw = 0.5;
                                       }
                                       
                                       return clamp(raw, 0.0, 1.0) * 10.0 - 5.0;
                                   }
                                   
                                   void main() {
                                       
                                       vec2 pos = gl_TexCoord[0].xy;
                                       
                                       if (isInit == 1) {
                                           
                                           float a = 0.5 + texture2DRect(tex0, pos).r;
                                           
                                           gl_FragColor = vec4(a, a, a, 1.0);
                                           
                                       } else if (isInit == 0) {
                                           
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
                                           
                                           float laplacian = (n + e + s + w - 4.0 * a) / (h * h);
                                           float bilaplacian = (n2 + e2 + s2 + w2 + 2.0 * (ne + se + sw + nw)
                                                                - 8.0 * (n + e + s + w) + 20.0 * a) / (h * h * h * h);
                                           float gradX = (e - w) / (2.0 * h);
                                           float gradY = (s - n) / (2.0 * h);
                                           float gradMagSqared = gradX * gradX + gradY * gradY;
                                           
                                           float delta = mu * a - (bilaplacian + 2.0 * laplacian + a) - (beta / 3.0) * (gradMagSqared + 2.0 * a * laplacian) - a * a * a;
                                           float result = a + timestep * delta;
                                           float v = (result + 5.0) / 10.0;
                                           
                                           gl_FragColor = vec4(v, v, v, 1.0);
                                       }
                                   }
        );
        
    };
    
    void init() {
        
        isInit = 1;
        update();
        isInit = 0;
    }
    
    void update() {
        
        pingPong.dst->begin();
        ofClear(0, 255);
        
        shader.begin();
        shader.setUniformTexture("backbuffer", pingPong.src->getTextureReference(), 0);
        shader.setUniformTexture("tex0", textures[0].getTextureReference(), 1);
        shader.setUniform1f("timestep", 0.0008f);
        shader.setUniform1f("h", 0.4f);
        shader.setUniform1f("mu", mu);
        shader.setUniform1f("beta", beta);
        shader.setUniform1i("isInit", isInit);
        renderFrame();
        shader.end();
        pingPong.dst->end();
        
        pingPong.swap();
    
    };
    
    float timestep, h, mu, beta, pattern;
    int isInit;
};