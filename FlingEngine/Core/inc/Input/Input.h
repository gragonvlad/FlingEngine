#pragma once

#include "NonCopyable.hpp"
#include "Input/Key.h"
#include <entt/signal/delegate.hpp>
#include <entt/signal/sigh.hpp>

namespace Fling
{
	/**
	 * @brief	represents the current mouse position in screen space
	 */
	struct MousePos
	{
		float X;
		float Y;
	};

	/**
	* Base input class for polling input in the Fling Engine
	*/
	class Input : public NonCopyable
	{
	public:

		/** Initialize the input instance on this platform. Handle and input mapping on this platform */
		static void Init() { m_Instance->InitImpl(); }
		
		/** 
		* @brief	PreUpdate is called before polling of input, and after Init. Useful for 
		*			anything that needs to happen after Window creation.
		*/
		static void PreUpdate() { m_Instance->PreUpdateImpl(); };

		/**
		* Shuts down input manager.
		* Deletes the input implementation pointer.
		*/
		static void Shutdown() 
		{ 
			m_Instance->ShutdownImpl();
			delete m_Instance; 
		}

		/**
		 * @brief Update any input polling that needs to happen on this platform.
		 */
		static void Poll() { m_Instance->PollImpl();  }
		
		static bool IsKeyDown(const std::string& t_KeyName) { return m_Instance->IsKeyDownImpl(t_KeyName); }
		static bool IsKeyHeld(const std::string& t_KeyName) { return m_Instance->IsKeyHelpImpl(t_KeyName); }
		
		static bool IsMouseButtonPressed(const std::string& t_KeyName) { return m_Instance->IsMouseButtonPressedImpl(t_KeyName); }
		static bool IsMouseDown(const std::string& t_KeyName) { return m_Instance->IsMouseDownImpl(t_KeyName); }

		/**
		 * Get the current mouse position in screen space
		 */
		static MousePos GetMousePos() { return m_Instance->GetMousePosImpl(); }
		
		/**
		 * @brief Bind a callback function to when a key is pressed 
		 * 
		 * @tparam Candidate The function that you would like to bind
		 * @param t_KeyName Key name to bind to 
		 * @param t_Instance The instance of the object you are binding to
		 * @see https://github.com/skypjack/entt/wiki/Crash-Course:-events,-signals-and-everything-in-between#delegate
		 * @see Fling::KeyNames
		 */
		template<auto Candidate, typename Type>
		static void BindKeyPress(const std::string& t_KeyName, Type& t_Instance);

		/** Input Key mappings */
		typedef std::map<std::string, Fling::Key> KeyMap;
		typedef std::pair<std::string, Fling::Key> KeyPair;

		/** Key press delegate mappings */
		typedef std::map<std::string, entt::delegate<void()>> KeyDownMap;
		typedef std::pair<std::string, entt::delegate<void()>> KeyDownMapPair;

	protected:

		/** The actual key map */
		static KeyMap m_KeyMap;

		static KeyDownMap m_KeyDownMap;

		/** Created by the implementation class @see WindowInput */
		static Input* m_Instance;
		
		/**
		 * @brief Add a key mapping to this platform. 
		 * 
		 * @param t_Name 		The name of this key 
		 * @param t_KeyCode 	The KeyCode that maps this key to the current platform
		 */
		static void AddKeyMap(const std::string& t_Name, UINT32 t_KeyCode)
		{
			m_KeyMap.insert( KeyPair(t_Name, Fling::Key(t_Name, t_KeyCode)) );
		}

		virtual void InitImpl() = 0;
		virtual void PreUpdateImpl() = 0;
		virtual void ShutdownImpl() = 0;

		/** Poll for input from the keyboard */
		virtual void PollImpl() = 0;

		virtual void InitKeyMap() = 0;

		virtual bool IsKeyDownImpl(const std::string& t_KeyName) = 0;
		virtual bool IsKeyHelpImpl(const std::string& t_KeyName) = 0;
		
		virtual bool IsMouseButtonPressedImpl(const std::string& t_KeyName) = 0;
		virtual bool IsMouseDownImpl(const std::string& t_KeyName) = 0;
		virtual MousePos GetMousePosImpl() = 0;
	};

}	// namespace Fling

#include "Input.inl"