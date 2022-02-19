#include "GraphicsContext.h"

namespace Clove {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(void* handle);
		void Init() override;
		void SwapBuffers() override;
	private:
		void* m_window_handle;
	};
}