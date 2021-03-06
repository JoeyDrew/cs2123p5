/* cs2123p5DannyN.c

File created by Danny Nguyen. 
Includes:
	-processCommand(Tree tree, QuoteSelection quote, char szInputBuffer)
	-defineValue(Tree tree, char szToken, NodeT *pNew)
	-
Purpose: 
	Contains files created by Danny Nguyen for the driver to run. 

	-processCommand: Reads the input data and adds them into the tree.
	-defineValue: determines the cases for DEFINE and inserts into the tree.
*********************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p5.h"

/*********** setRoot *************
NodeT setRoot(Tree tree)
Purpose:
	Creates the root of the tree so that the first options inserted
	into the tree know where the root of the tree is.
Parameters:
	I 	Tree tree 				The tree to insert the root into.
Returns: None.
Notes: 
	[11/8/2015 11:12PM]: No notes as of right now. Might add some later.
*********************************/
void setRoot(Tree tree)
{
	NodeT *pNew = malloc(sizeof(NodeT));
	char szId[MAX_TOKEN_SIZE - 1] = "ROOT";
	// set the values for the root element.
	strcpy(pNew->element.szId, szId);			// unique ID named ROOT so that ID's being put into
											// the tree can find the root.
	strcpy(pNew->element.szTitle, "Price Menu");	// Name of the tree to print what the tree is for.
	// set both pointers to NULL.
	pNew->pChild = NULL;
	pNew->pSibling = NULL;
	// inserts into the tree. Passes in the "ROOT" search to see if one exists already.
	insertPriceMenu(tree, pNew->element, "ROOT");
}

/********** defineCommand ************
int defineCommand(char szToken[])
Purpose:
	Defines a token and assigns it an int value for switch statement.
Parameters:
	I char szToken[] 			token to check
Returns:
	Returns an integer according to the token passed in.
Notes:
	None.
*************************************/
int defineCommand(char szToken[])
{
	if (strcmp(szToken, "DEFINE"))
		return DEFINE;
	else if (strcmp(szToken, "QUOTE"))
		return QUOTE;
	else if (strcmp(szToken, "PRINT"))
		return PRINT;
	else if (strcmp(szToken, "DELETE"))
		return DELETE;
	else
	{
		printf("Error: Command not found\n");
		exit (-1);
	}
}
/********* allocateNodeT *********
NodeT *allocateNodeT(Element value)
Purpose:
	This will allocate a new node and return the node with the values.
Parameters:
	I Element value 		value to insert into node.
Returns:
	Returns the newly allocated node.
Notes: 
	(11/5/2015 2:00PM): Possibly move this function into the main driver so that
	everyone who needs to allocate a new node may use this function.
*********************************/
NodeT *allocateNodeT(Element value)
{
	NodeT *pNew = (NodeT *)malloc(sizeof(NodeT));
	pNew->element = value;
	pNew->pChild = NULL;
	pNew->pSibling = NULL;
	return pNew;
}

/********** defineValue ************
void defineValue(Tree tree, char szToken, NodeT *pNew, char pszRemainingTxt)
Purpose:
	This will check to see what the second token is after the DEFINE token is detected. 
	If the second token is OPTION:
		The input line will contain:
		DEFINE OPTION szId szSubordinateToId szTitle
		where:
			szId:
				This is the ID of the new node. 
			szSubordinateToId:
				This is the ID of the node that is its parent (via a pChild Pointer).
				Note: If we are inserting at the root, then the szSubordinateToId is going
				to be "ROOT".
			szTitle:
				This is the title for the node. (e.g. "Model").

		DEFINE VALUE szId szOptionId cCostInd dCost szTitle
		where:
			szId:
				This is the ID of the new node.
			szOptionId:
				This is the ID of the option which is its parent (via a pChild pointer).
			cCostInd:
				This is a character that represents if it has a cost or not. 
				Note: 'Y' indicates that it has a value. 'N' indicates that it does not have a value.
			dCost:
				This is the cost of the ID, which may be a positive or a negative number.
				Note: This is assuming that the cCostInd is equal to 'Y'.
			szTitle:
				This is the title for the node. (e.g. "Racing White").
Parameters:
	I 	Tree tree 					This is the tree that the information is going to be inserted into.
	I 	char szToken 				This is the token that getToken will insert the token that it is
									is grabbing into.
	I   NodeT *pNew 				This is the newly created node. (Possibly edited to make this a local
									value).
	I 	pszRemainingTxt				This is the remaining text after grabbing the input line.

Returns: None.
Notes: May add some extra notes later. Currently (11/5/2015 1:59PM) no notes need to be added.
***********************************/
void defineValue(Tree tree, char szToken[], NodeT *pNew, char *pszRemainingTxt)
{
	char szSubordinateToId[MAX_ID_SIZE]; 		// used to store the subordinate ID. (DEFINE OPTION _____ xxxxx _____) <-- Where the xxxxx is located.
	

	// if the command line is DEFINE, then check the second token.
	if (strcmp(szToken, "OPTION"))
	{
			// set the cNodeType of the element to O, indicating that it is an OPTION
			pNew->element.cNodeType = 'O';

			// grab the next token, and insert it into the ID of the new element. 
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(pNew->element.szId, szToken);

			// this will grab the subordinate ID from the input line.
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(szSubordinateToId, szToken);
			
			// This will grab the title name of the input line.
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(pNew->element.szTitle, szToken);

			// Insert the new node into the tree. 
			insertPriceMenu(tree, pNew->element, szSubordinateToId);
	}

	else if (strcmp(szToken, "VALUE"))
	{
			// set the cNodeType of the element to V, indicating that it is a VALUE
			pNew->element.cNodeType = 'V';

			// Grabs the ID of the value
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(pNew->element.szId, szToken);

			// Grabs the subordinate ID to know where it is inserted into.
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			strcpy(szSubordinateToId, szToken);

			// Grabs the cost index to determine if there is a value.
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			pNew->element.cCostInd = szToken[0];

			// grabs the cost of the value.
			if (szToken[0] == 'Y')
			{
				pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
				pNew->element.dCost = atoi(szToken);
			}

			// the rest of the text remaining will be the title that is printed.
			if (pszRemainingTxt != NULL)
			{
				strcpy(pNew->element.szTitle, pszRemainingTxt);
			}
			// if there is no title, print an error then break.
			else
			{
				printf("Error: No title identified.");
				return;
			}

			// insert the information into the Price Menu.
			insertPriceMenu(tree, pNew->element, szSubordinateToId);
	}
	else
		ErrExit(ERR_COMMAND_LINE, "Define value not recognized");
}


/************ printPriceMenu **************
void printPriceMenu(NodeT *p, int iIndent)
Purpose:
	Prints out the information inside the tree.
Parameters:
	I NodeT *p 				Top node of the tree.
	I int iIndent 			Used for nicely indenting the information
Returns: None.
Notes: None.
******************************************/
void printPriceMenu(NodeT *p, int iIndent)
{
	int i;
	if (p == NULL)
		return;
	printPriceMenu(p->pChild, iIndent + 1);
	printf("\t");
	for (i = 0; i < iIndent; i++)
	{
		printf("\t");
	}
	if (p->element.cCostInd == 'Y')
	{
		printf("%-8s%50lf\n",p->element.szTitle, p->element.dCost);
	}
	else
	{
		printf("%-8s\n", p->element.szTitle);
	}
	printPriceMenu(p->pSibling, iIndent + 1);
}
/************* insertPriceMenu ****************
void insertPriceMenu(Tree tree, Element element, char szParentId[])
Purpose:
	Inserts the information into the tree. 
Parameters:
	I Tree tree 				The tree the info is being inserted into
	I Element element 			The information that is being inserted into the tree
	I char szParentId[] 		The parent that needs to be found. If it isn't, then it'll be inserted as
								a new child.
Returns: None.
Notes: Function created by Joey Drew.
**********************************************/
void insertPriceMenu (Tree tree, Element element, char szParentId[])
{
   if (tree == NULL)
       return;
 
   NodeT *node;

   if ((node = findId(tree->pRoot, szParentId)) == NULL)
       return;
 
   if (node->pChild == NULL)
   {
       NodeT *child = allocateNodeT (element);
       node->pChild = child;
       return;
   }
 
   node = node->pChild;
   while (node->pSibling != NULL)
   {
       node = node->pSibling;
   }
 
   NodeT *sibling = allocateNodeT (element);
   node->pSibling = sibling;
}
/************ findId **************
NodeT *findId(NodeT *p, char szId[])
Purpose:
	Searches for an ID in the tree. if not found, returns nothing.
Parameters:
	I NodeT *p 			The starting node to start searching
	I char szId[] 		The ID to search for
Returns:
	Returns the node inside the tree that it is looking for.
Notes: Function created by J'hon 
**********************************/
NodeT *findId(NodeT *p, char szId[])
{
        if (p == NULL)
                return NULL;
        if (strcmp(p->element.szId, szId) == 0)
                return p;
        if (p->pChild != NULL)
                return findId(p->pChild, szId);
        return findId(p->pSibling, szId);
}

/************ findParent **************
NodeT *findParent(NodeT *pParent, NodeT *p, NodeT *pKid)
Purpose:
Returns the parent of a given node. Returns NULL if not found or doesn't have a parent.
Parameters:
I/O	NodeT	*pParent			//current parent level
I/O	NodeT	*p					//tree of options
I/O	NodeT	*pKid				//child used to find the parent
I	NodeT	*pCheck				//checks to see if pKid exists in tree
I	NodeT	*pSiblingCheck		//recursively searches through siblings Nodes
I	NodeT	*pChildCheck		//recursively searches through children Nodes
Returns:
NULL	- Not found, No Parent
pParent - Returns the found parent held in pParent
p		- Returns the current Node in p if it is the parent.
Notes: None.
******************************************/
NodeT *findParent(NodeT *pParent, NodeT *p, NodeT *pkid)
{
	//Node checkers
	NodeT *pCheck, *pSiblingCheck, *pChildCheck;
	//checks for NULL p
	if (p == NULL)
		return NULL;
	//checks to see if a pkid is at the root of the tree
	if (pParent == NULL && strcmp(p->element.szId, pkid->element.szId) == 0)
		return NULL;
	//checks for pkid in tree
	pCheck = findId(p, pkid->element.szId);
	//continue checking
	if (pCheck != NULL)
	{
		//checks if pParent has been found
		if (pParent != NULL && strcmp(p->element.szId, pkid->element.szId) == 0)
			return pParent;
		//recursively search siblings
		pSiblingCheck = findParent(pParent, p->pSibling, pkid);
		//recursively search children
		pChildCheck = findParent(p, p->pChild, pkid);
		//checks for NULL sibling checks
		if (pSiblingCheck != NULL)
			return pSiblingCheck;
		//checks for NULL child checks
		if (pChildCheck != NULL)
			return pChildCheck;

	}
	//else return NULL
	return NULL;

}

/******** processCommand *********
void processCommand(Tree tree, QuoteSelection quote, char szInputBuffer)
Purpose:
	Processes the command for the main file 
Parameters:
	I Tree tree 						tree to insert information into
	I QuoteSelection quote 				quote function use. ???
	I char szInputBuffer 				used to read input file lines.
Returns: None
Notes: 
	(11/5/2015 2:00PM): Quote, Delete, and Print will be written in another file. These cases
	have not yet been added, thus will not do anything until they are created.
*********************************/
void processCommand(Tree tree, QuoteSelection quote, char szInputBuffer[])
{
	NodeT *pNew = malloc(sizeof (NodeT));
	char szToken[MAX_TOKEN_SIZE];
	char *pszRemainingTxt;
	int iSwitchCode;

	// grabs the first token of the input line
	pszRemainingTxt = getToken(szInputBuffer, szToken, MAX_TOKEN_SIZE - 1);
	iSwitchCode = defineCommand(szToken);

	// determines what to do from first command line
	switch(iSwitchCode)
	{
		// if case is DEFINE
		case DEFINE:
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			defineValue(tree, szToken, pNew, pszRemainingTxt);
			break;

		// if case is QUOTE
		case QUOTE:
			break;

		// if case if DELETE		
		case DELETE:
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			//deleteItem(tree, szToken);
			break;

		// if case is PRINT
		case PRINT:
			pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
			if (strcmp(szToken, "ALL"))
			{
				printf("%-6s\n", tree->pRoot->element.szTitle);
				printPriceMenu(tree->pRoot->pChild, 0);
			}
			else if (strcmp(szToken, "ONE"))
			{
				pszRemainingTxt = getToken(pszRemainingTxt, szToken, MAX_TOKEN_SIZE - 1);
				//printOne(tree, szToken);
			}
			else
				ErrExit(ERR_COMMAND_LINE, "PRINT command not recognized");

			//printOptions(tree, szToken);
			break;

		// if case is unrecognized, return an error. 
		default:
			ErrExit(ERR_COMMAND_LINE, "command not recognized");
	}

}



