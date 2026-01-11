#pragma once

// project
#include "ball.h"
#include "utils.h"

// third party

// std

gui::mesh::ball::ball()
{
    const GLuint rings = 128;
    const GLuint segments = 128;
    const GLfloat radius = 1.0f;

    for (GLuint ring = 0; ring <= rings; ++ring) {
        const GLfloat phi = (static_cast<GLfloat>(ring) / rings) * M_PI;
        
        for (GLuint segment = 0; segment <= segments; ++segment) {
            const GLfloat theta = static_cast<GLfloat>(segment) / segments * 2.0f * M_PI;
            
            // Spherical to Cartesian
            const GLfloat x = radius * sin(phi) * cos(theta);
            const GLfloat y = radius * cos(phi);  // Y is up
            const GLfloat z = radius * sin(phi) * sin(theta);
            
            const Eigen::Vector3f vertex{x, y, z};
            utils::vertex_push_back(m_vertices, vertex);
        }
    }

    // Generate indices - simple quad subdivision
    for (GLuint ring = 0; ring < rings; ++ring) {
        for (GLuint segment = 0; segment < segments; ++segment) {
            // Calculate indices for the quad
            const GLuint first = ring * (segments + 1) + segment;
            const GLuint second = first + 1;
            const GLuint third = first + (segments + 1);
            const GLuint fourth = third + 1;
            
            // Make two triangles for the quad
            m_indices.insert(m_indices.end(), {first, third, second});
            m_indices.insert(m_indices.end(), {second, third, fourth});
        }
    }
}
