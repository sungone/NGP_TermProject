#include "ViewerPlayer.h"

void ViewerPlayer::drawP(GLuint shaderProgramID)
{
	glUniform3f(glGetUniformLocation(shaderProgramID, "fColor"), color.r, color.g, color.b);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));

}

void ViewerPlayer::updateViewerPlayer(const glm::vec3& newPos, const glm::vec3& newColor, const glm::vec3& newScale)
{
	pos.x = newPos.x;
	pos.y = newPos.y;
	pos.z = newPos.z;
	
	color.r = newColor.r;
	color.g = newColor.g;
	color.b = newColor.b;

	scale.x = newScale.x;
	scale.y = newScale.y;
}

void ViewerPlayer::render(GLuint shaderProgramID)
{
	glBindVertexArray(vao);

	model = glm::mat4(1.f);
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);

	drawP(shaderProgramID);
}
