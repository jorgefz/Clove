#include "GraphicsContext.h"

struct GLFWwindow;

namespace Clove {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* window_handle);
		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_window_handle;
	};
}