#pragma once

#include <vector>
#include "../Day4/Entities.h"

// Object Pool Design Pattern
class EntitiesPool : public IObserver
{
private:
	int _count;

public:
	std::vector<EntityPtr> _pool;
	std::vector<bool> _isInUse;

	EntitiesPool(std::string const& animationsPath);

	void insert(EntityPtr whom);

	// throws exception if all entities are in use.
	EntityPtr getNext();

	void returnEntity(EntityPtr toFree);

	int getCount();
	bool isFree(int idx);


	// Inherited via IObserver
	virtual void onNotify(Event const& e) override;

};

