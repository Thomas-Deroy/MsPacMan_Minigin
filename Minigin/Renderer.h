#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */

	struct RenderParams {
		float x = 0.f;
		float y = 0.f;
		float width = 0.f;
		float height = 0.f;
		SDL_Color color = { 255, 255, 255, 255 };
		float rotation = 0.f;
		SDL_RendererFlip flip = SDL_FLIP_NONE;
		bool useRotation = false;
	};

	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, const RenderParams& params) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& dstRect, const SDL_Rect& srcRect, double angle, SDL_RendererFlip flip) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

