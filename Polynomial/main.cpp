/*
"Ethics Statement: Other than anything noted in the disclosures
below, the code submitted is of my design and my design alone.
I recieved no help nor used any references, resources, and had no
discussion with other classmates.
I understand the University policy concerning ethics and
understand that violating this policy will result in drastic repercussions.

Disclosures: Please note any discussions, use of supplemental
resources, use of pre-existing code including programming language
libraries. (The use of pre-existing code is not encouraged and you may not receive full credit as a result,
but you will not be in violation of the Ethics Policy, if you disclose.)
If you have nothing to disclose, please type "no disclosures".
Discussion between students is encouraged (no penalty), but please disclose here:

-I referred to my previous code from 052 and P1
-I discussed concepts on multiply and ways to do exponentiation quickly with my classmates 
-I discussed ways to manipulate on the bit level to speed up Pow function with my classmates
*/

# include "main.h"


int main(int argc, char *argv[]) {
	try
	{

		if (argc < 2)
			cout << "invalid usage: " << argv[0] << " <filename>\n";
		else {
			ifstream inputFile(argv[1]);
			if (!inputFile.is_open())
				cout << "Could not open file\n";
			else
			{
				Poly<long int> Poly1;
				Poly<long int> Poly2;

				string line;
				long int coefficient;
				int exponent;

				// get first line to populate Poly1 
				getline(inputFile, line);

				istringstream iss(line);

				while (iss >> skipws >> coefficient >> exponent)
				{
					Poly1.addLast(coefficient, exponent);
				}

				//get second line for second polynomial
				getline(inputFile, line);

				istringstream iss2(line);

				while (iss2 >> skipws >> coefficient >> exponent)     
				{
					Poly2.addLast(coefficient, exponent);
				}

				cout << "Poly1 is ";
				Poly1.display();
				cout << "Evaluating Poly1 with x=10: " << Poly1.eval(10) << endl << endl;
		
				cout << "Poly2 is ";
				Poly2.display();
				cout << "Evaluating Poly2 with x=10: " << Poly2.eval(10) << endl << endl;

				cout << "Poly3: Poly1 and Poly2 added together is " << endl;
				Poly<long int> Poly3;
				Poly3 = Poly1 + Poly2;
				Poly3.display();
				cout << "Evaluating Poly3 with x=10: " << Poly3.eval(10) << endl << endl;

				cout << "Poly4: Poly1 and Poly2 multiplied together is " << endl;
				Poly<long int> Poly4;
				Poly4 = Poly1.multiply(Poly2);
				Poly4.display(); 
				cout << "Evaluating Poly4 with x=2: " << Poly4.eval(2) << endl << endl;


				cout << "Poly1 to the 3rd power: " << endl; 
				Poly<long int> P1exp = Poly1.exponentiation(3);
				P1exp.display();  

				int Poly2Eval = Poly2.eval(5); 
				cout << "Poly2 evaluated for x = 5: " << Poly2Eval << endl;
				cout << "Poly1 to the " << Poly2Eval << "th power: " << endl;
				Poly<long int>P1exp2 = Poly1.exponentiation(Poly2Eval);
				P1exp2.display(); 
			}

			inputFile.close();
		}
	}
	
	//try/catch is not needed since I handle all the errors in my function calls, 
	//but just to make sure I will leave this here
	catch (string s)
	{
		cout << s << endl;
	}
	
	cin.get();
	return 0;
}