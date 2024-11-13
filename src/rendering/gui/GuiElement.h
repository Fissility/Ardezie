#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class GuiElement {
private:

	std::vector<GuiElement*> removeBuffer;
	std::vector<GuiElement*> addBuffer;

protected:

	std::vector<GuiElement*> children;
	GuiElement* parent;
	sf::Vector2f size = sf::Vector2f(0,0);
	sf::Vector2f position = sf::Vector2f(0, 0);
	sf::Vector2f offset = sf::Vector2f(0, 0);
	bool renderStale = false;

public:
	// The position transform for rendering
	sf::Transform positionTransform;
	// TRUE if it should be visible and interactible
	bool visible = true;
	// TRUE if the element needs to be killed
	bool old = false;

	~GuiElement() {
		for (GuiElement* e : children) delete e;
	}

	/*
	* @brief Sets a gui element to be the parent
	*/
	void setParent(GuiElement* element) {
		this->parent = element;
		recalculateTransofrm();
	}

	/*
	* @brief Adds a child gui element, the removing happens at the next call of update()
	*/
	void addChild(GuiElement* element) {
		addBuffer.push_back(element);
	}

	/*
	* @brief Removes and deletes a child gui element, the removing happens at the next call of update()
	*/
	void removeChild(GuiElement* element) {
		removeBuffer.push_back(element);
	}

	/*
	* @brief Recalculates all related position transforms, call if manually updating position.
	*/
	void recalculateTransofrm() {
		positionTransform = sf::Transform::Identity;
		positionTransform.translate(getActualPosition());
		positionTransform.scale(sf::Vector2f(1, 1));
		for (GuiElement* e : children) e->recalculateTransofrm();
	}

	/*
	* @brief Triggered by the SFML window events
	*/
	virtual void onWindowEvent(sf::Event& event) {
		for (GuiElement* e : children) e->onWindowEvent(event);
	};
	/*
	* @brief Triggered when the screen needs to be drawns
	*/
	virtual void onWindowDraw(sf::RenderWindow& window) {
		renderStale = false;
		for (GuiElement* e : children) e->onWindowDraw(window);
	};
	/*
	* @brief Triggered when the element needs to be resized
	* @param newSize = The new size of the element
	*/
	virtual void onResize(sf::Vector2f newSize) {};
	/*
	* @return Returns the size of the element
	*/
	virtual sf::Vector2f getTotalSize() { return sf::Vector2f(); };
	/*
	* @brief Triggered when the display offset of the 
	*/
	virtual void setOffset(sf::Vector2f offset) {
		this->offset = offset;
		recalculateTransofrm();
	};
	virtual void setPos(sf::Vector2f newPos) {
		this->position = newPos;
		recalculateTransofrm();
	}
	/*
	* @brief Used for logic update, called after the events and before rendering
	*/
	virtual void update() {
		for (GuiElement* e : removeBuffer) {
			children.erase(std::find(children.begin(), children.end(), e));
			delete e;
			renderStale = true;
		}
		removeBuffer.clear();
		for (GuiElement* e : addBuffer) {
			children.push_back(e);
			renderStale = true;
		}
		addBuffer.clear();
		for (GuiElement* e : children) {
			e->update();
			if (e->old) removeChild(e);
		}
	}
	/*
	* @return TRUE if the element needs to be rendered again
	*/
	virtual bool isRenderStale() {
		if (renderStale) return true;
		for (GuiElement* e : children) if (e->isRenderStale()) return true;
		return false;
	}
	void setStale() {
		renderStale = true;
	}
	/*
	* @return Returns the position at which the element should be rendered at
	*/
	sf::Vector2f getActualPosition() {
		return position + offset + (parent != nullptr ? parent->getActualPosition() : sf::Vector2f(0,0));
	}
	/*
	* @return Returns the relative position of the element
	*/
	sf::Vector2f getPosition() {
		return position;
	}
	/*
	* @return Returns the offset of the element in relation the relative position
	*/
	sf::Vector2f getOffset() {
		return offset;
	}
	/*
	* @return Returns the size of the element
	*/
	sf::Vector2f getSize() {
		return size;
	}

};

