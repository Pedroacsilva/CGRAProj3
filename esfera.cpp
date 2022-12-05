CGRASphere::CGRASphere() {
  /*    Eqs paramétricas
x = (r * cos(phi) * cos(theta))
y = (r * cos(phi) * sin(theta))
z = r * sin(phi)                */

  // Generate & Bind VAO
  glGenVertexArrays(1, &sphere_VAO);
  glBindVertexArray(sphere_VAO);
  /*    Eqs paramétricas
  x = (r * cos(phi) * cos(theta))
  y = (r * cos(phi) * sin(theta))
  z = r * sin(phi)                */
  const float radius = 1.0f;
  const float PI = 3.14f;
  float x, y, z, xy;
  // float nx, ny, nz;
  float u, v;
  float sectorStep = 2 * PI / 40;
  float stackStep = PI / 40;
  float sectorAngle, stackAngle;
  std::vector<float> vtx_info;

  for (int i = 0; i <= 40; i++) {
    stackAngle = PI / 2 - i * stackStep; // Começar no pólo norte
    xy = radius * std::cos(stackAngle);
    z = radius * std::sin(stackAngle);
    for (int j = 0; j <= 40; j++) {
      sectorAngle = j * sectorStep;
      x = xy * std::cos(sectorAngle);
      y = xy * std::sin(sectorAngle);
      // XYZ
      vtx_info.emplace_back(x);
      vtx_info.emplace_back(y);
      vtx_info.emplace_back(z);
      // V
      u = static_cast<float>(j) / 40;
      v = static_cast<float>(i) / 40;
      vtx_info.emplace_back(u);
      vtx_info.emplace_back(v);
      // RGBA
      vtx_info.emplace_back(0.5f);
      vtx_info.emplace_back(0.5f);
      vtx_info.emplace_back(0.5f);
      vtx_info.emplace_back(1.0f);
      // Normals
      vtx_info.emplace_back(x);
      vtx_info.emplace_back(y);
      vtx_info.emplace_back(z);
    }
  }
  // Prepare IBO
  std::vector<int> indices;
  int k1, k2;
  for (int i = 0; i < 40; ++i) {
    k1 = i * 41;
    k2 = k1 + 41;
    for (int j = 0; j < 40; ++j, ++k1, ++k2) {
      if (i != 0) {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }
      if (i != 39) {
        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }

  // Generate, bind and fill VBO
/*  glGenBuffers(1, &sphere_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, sphere_VBO);
  glBufferData(GL_ARRAY_BUFFER, vtx_info.size() * sizeof(float),
               vtx_info.data(), GL_STATIC_DRAW);

  // Generate, bind and fill IBO
  glGenBuffers(1, &sphere_IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);*/

  unsigned long int offset = 0;
  unsigned int stride = (3 + 2 + 4 + 3) * sizeof(GL_FLOAT);
  // Define Vertex Attrib Array and specify data layout
  // Model XYZ coordinates

/*  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
  glEnableVertexAttribArray(0);
  offset += 3 * sizeof(GL_FLOAT);

  // Model UV texture coordinates
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
  glEnableVertexAttribArray(1);
  offset += 2 * sizeof(GL_FLOAT);

  // Model RGBA values
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
  glEnableVertexAttribArray(2);
  offset += 4 * sizeof(GL_FLOAT);

  // Model Normals
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (const void *)offset);
  glEnableVertexAttribArray(3);
  offset += 3 * sizeof(GL_FLOAT);*/

  std::cout << "[SPHERE]VAO: " << sphere_VAO << ", VBO: " << sphere_VBO << ", IBO: " << sphere_IBO
            << ".\n";
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

CGRASphere::~CGRASphere() {}

void CGRASphere::drawIt(glm::mat4 V, glm::mat4 P) {
  //glBindVertexArray(sphere_VAO);
  //glBindVertexArray(sphere_VBO);
  //glBindVertexArray(sphere_IBO);
  //shader->Bind();
  glm::mat4 vp = P * V;
  //shader->SetUniformMat4f("u_VP", vp);
  //shader->SetUniformMat4f("u_Model", modeltr);

    //std::cout << "[SPHERE DRAW CALL]VAO: " << sphere_VAO << ", VBO: " << sphere_VBO << ", IBO: " << sphere_IBO
    //        << ".\n";

//  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}