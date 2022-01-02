#include "Renderer.h"
#include "utils_debug.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    shader.bind();
    //shader.SetUniform4f("u_Color", 0.4f, 0.3f, 0.8f, 1.0f);

    va.bind();
    ib.bind();

    GL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Renderer::Clear() const
{
    GL(glClear(GL_COLOR_BUFFER_BIT));
}
