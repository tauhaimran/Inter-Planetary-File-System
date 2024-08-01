#pragma once
#include <iostream>
#include <string>
#include "fileHandler.h"
//#include "DHT_RING.h"
#include "dhtNodes.h"

class router {

private:

	std::string traversed_path; //path to be sent into history_mech.
	bool startup; //bool to check if a routing is in session

public:

	//constructor...
	router() :traversed_path(""), startup(false) {}


	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

		//takes FT and returns the next node...
	void route_next_node(Node*& currnt, std::string ID, BigInteger& srch_val) {



		FingerTable* cmp = currnt->getFingertable();
		FingerTable* cmprev = nullptr;
		BigInteger ourId(ID);

		//...........................................................
		   //string for traversing history former...
		if (!startup) {
			traversed_path = currnt->getId().value;
			startup = true;
		}
		std::cout << "\n\n >> " << traversed_path;
		//std::cout << "\nroute_next_node\n";
	 //...........................................................
		//traverse the FT while the current hash value/ID is lesser than the id being searched...
		while (cmp->next != nullptr && cmp->getMachineId() < ourId) {

			//if actual node exists in the FT
			if (ourId == cmp->getMachineId()) {
				break;
			}

			cmprev = cmp; //prev ptr tracking...
			cmp = cmp->next; //move to next node in FT...

		}
		std::cout << "\nroute_next_node\n";
		//...........................................................
		   //if the last value of the finger table
		if (cmp->next == nullptr) {

			//if the last id is greater than the id we're looking for...
			if (cmp->pointerToNode->id > ourId) {
				cmp = cmprev;
			}

		}//if not the first value is the one we're looking for...
		else if (cmprev != nullptr) {
			cmp = cmprev;
		}
		//...........................................................
		traversed_path += "->";
		traversed_path += cmp->getMachineId().value;

		std::cout << "\n >> " << traversed_path;
		//...........................................................

				//we've found the new routing address... 
		currnt = cmp->pointerToNode;//updating the traversing pointer...		
		srch_val = currnt->getId(); //updating the value we're searching for...

		return;
	}

	void display_routing_track() { std::cout << "\n\n >> " << traversed_path << "\n\n"; }


	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-


		//stores and clears routing history buffers...
	void terminate_routing(bool result) { // true - succesfull...

		if (result) { traversed_path += " | routing succesful... "; }
		else { traversed_path += " | routing failed... "; }
		traversed_path += '\0'; //ending history...

		//tesing cout calls...
		writeToFile(traversed_path, filename_log_IPFS);
		writeToFile(traversed_path, filename_log_ROUTER);
		std::cout << "\n\n >> " << traversed_path << "\n\n";
		traversed_path = "";
		startup = false;
	}


	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-


		//routes the node pointer to the user selected machine...
	Node* route_to_node(Node* start = nullptr, BigInteger endID = BigInteger("0")) {

		std::cout << "\nroute_to_node\n"; \

			Node* result = start;

		//ambiguity checks...
		if (result == nullptr) { return nullptr; }
		if (result->prev == nullptr) { return result; }
		if (result->next == nullptr) { return result; }


		if (start == nullptr) { // if the arguments passed are incorrected
			std::cout << "\n ::: ERROR route_to_node(NULL,x) :::\n";
			return nullptr;
		}
		std::cout << "--check1";
		if (endID == BigInteger("0")) {
			traversed_path = start->getId().value;
			return start;
		}
		std::cout << "--check2";
		if (endID <= start->getId()) {
			traversed_path = start->getId().value;
			return start;
		}
		std::cout << "--check3";

		//if same id's sent return 0-jump history
		if (endID == start->getId()) {
			traversed_path = start->getId().value;
			return start;
		}

		//if in prev set behind zero or an out of range typo...
		if (endID > start->prev->getId()) {
			traversed_path = start->getId().value;
			return start;
		}

		//if behind the start of dht and >= 0 && <= start->getId()
		if (endID <= start->prev->getId() && endID > start->prev->prev->getId()) {
			traversed_path = start->prev->getId().value;
			return start->prev;
		}

		//if in the set of end...
		if (endID == start->prev->getId()) {
			traversed_path = start->prev->getId().value;
			return start->prev;
		}

		//...........................................................
		FingerTable* cmp = start->getFingertable();
		FingerTable* cmprev = nullptr;
		BigInteger ourId((endID.value));
		Node* trvser = start;

		//in b/w start and end nodes case - X...
		if (ourId > start->prev->getId() && ourId > start->getId() || ourId <= start->getId()) {
			return start;
		}

		std::cout << "--check4";

		while (!(endID <= ourId && trvser->prev->getId() < endID)) {

			std::cout << "--checkx";
			route_next_node(trvser, endID.value, ourId);

			// if the node found via finger tabling
			if (trvser == start && endID == start->getId()) {
				terminate_routing(true);
				return trvser;
			}



		}
		std::cout << "--check5";
		//traversing finger table to get the values...
		//while (cmp->next != nullptr && cmp->getMachineId() < ourId) {
		//	cmprev = cmp;
		//	cmp = cmp->next;
		//}

		////value checks in FT
		//if (cmp->getMachineId() <= ourId) {
		//	trvser = cmp->pointerToNode;
		//}
		//else {
		//	trvser = cmprev->pointerToNode;
		//}

		//if ( BigInteger(Machineid) <= id && curr->prev->getId() < BigInteger(Machineid) )

		result = trvser;

		//...........................................................
		terminate_routing(true);

		std::cout << "\nXXX route_to_node\n";
		return result;
	}





	bool is_in_correct_set(Node* start_of_ring, Node* curr, BigInteger Id, BigInteger MAX) {

		if (start_of_ring == nullptr) { return true; }
		if (curr == nullptr) { return false; }


		//if our node is the start of the ringDHT...
		if (start_of_ring == curr) {

			// in set 0->start-id
			if (Id <= curr->getId()) {
				return true;
			}

			if (Id > curr->prev->getId() && Id < MAX) {
				return true;
			}

		}


		//if our node is the start of the ringDHT...
		if (start_of_ring->prev == curr) {

			if (Id > curr->prev->getId() && Id < MAX && Id <= curr->getId()) {
				return true;
			}

		}


		//if somewhere in the middle...
		if (Id > curr->prev->getId() && Id < MAX && Id <= curr->getId()) {
			return true;
		}


		return false;

	}
};
