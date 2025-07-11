#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

// global variable
string state__1, state__2;
double optimal, inside, outside, delta__t;
void menu();

ofstream dataTable("data.txt");

// for 2 digit float
double roundTwoDigits(double num)
{
    return (round(num * 1000) / 10'00);
}

class Room
{
public:
    const int R__wall = 150;
    double D__T__wall(double out, double in, double dt)
    {
        return roundTwoDigits(((out - in) / R__wall) * dt);
    }
};

class Window
{
public:
    int R__window; // close by defualt
    double D__T__window(double out, double in, double dt, string condition)
    {
        if (condition == "1")
        {
            R__window = 45;
        }
        else if (condition == "2")
        {
            R__window = 50;
        }
        else if (condition == "3")
        {
            R__window = 60;
        }

        return roundTwoDigits(((out - in) / R__window) * dt);
    }
};

class airConditioner
{
public:
    double D__T__airCondiotioner(double dt, string condition)
    {
        double p = 0.4; //=> 8,000 /20,000
        if (condition == "cooler")
        {
            p *= -1;
        }
        else if (condition == "off")
        {
            p = 0;
        }
        return roundTwoDigits(p * dt);
    }
};

// for set and change state of windows
void setState()
{
    // first window
    cout << "Enter State of First Window" << endl;
    cout << "1_open" << endl;
    cout << "2_half-open" << endl;
    cout << "3_close" << endl;

    cout << "Enter Your Choice: ";
    cin >> state__1;
    // state__1 = setFinalState(state__1);

    // second window
    cout << "Enter State of second Window" << endl;
    cout << "1_open" << endl;
    cout << "2_half-open" << endl;
    cout << "3_close" << endl;

    cout << "Enter Your Choice: ";
    cin >> state__2;
    // state__2 = setFinalState(state__2);

    menu();
}

// for set state cooler or heater
string setAirCondition(double opt, double in)
{
    string airCondition;
    if (opt - in < -0.2) //=> 0.2 for make system more real
    {
        airCondition = "cooler";
    }
    else if (opt - in > 0.2)
    {
        airCondition = "heater";
    }
    else //=> if distance between optimal and inside is lowwer than 0.2 heater and cooler will turn off
    {
        airCondition = "off";
    }
    return airCondition;
}

// for print data
void printData(double time, double in, double opt, double wall, double window, double ch, string airCondition)
{
    cout << "Time: " << "\t" << "T__in: " << "\t" << "T optimal: " << "\t" << "DT_wall: " << "\t" << "DT_window: " << "\t" << "DT_cooler,heater" << "\t" << "air conditioner: " << endl;
    cout << time << "\t" << in << "\t" << opt << "\t" << "\t" << wall << "\t" << "\t" << window  << "\t" << "\t" << ch << "\t" << "\t" << "\t" << airCondition << endl;

    dataTable << "Time: " <<time<< " | " << "T__in: " <<in<< " | " << "T optimal: " <<opt<< " | " << "DT_wall: "<<wall << " | " << "DT_window: "<<window << " | " << "DT_cooler,heater" <<ch<< " | " << "air conditioner: " <<airCondition<< endl;
}

void simulation()
{
    Room r;
    Window w;
    airConditioner a;

    string isStop;
    double time = 0;

    while (isStop != "exit")
    {
        string airCondition = setAirCondition(optimal, inside);

        double DT_wall = r.D__T__wall(outside, inside, delta__t);
        double DT_window_1 = w.D__T__window(outside, inside, delta__t, state__1);
        double DT_window_2 = w.D__T__window(outside, inside, delta__t, state__2);
        double DT_ch = a.D__T__airCondiotioner(delta__t, airCondition);

        double result = DT_wall + DT_window_1 + DT_window_2 + DT_ch;

        printData(time, inside, optimal, DT_wall, DT_window_1+ DT_window_2, DT_ch, airCondition);

        time += delta__t;
        inside += result;

        cin >> isStop;
    }
    menu();
}

void menu()
{
    cout << "Room Temperature Simulation and Contorol Program" << endl;
    cout << endl;

    cout << "1_Change Optimal Temperature" << endl;
    cout << "2_Change State of Windows" << endl;
    cout << "3_Start Simulation" << endl;
    cout << "4_Exit" << endl;

    string choice;
    cout << "Enter Your Choice: ";
    cin >> choice;

    if (choice == "1")
    {
        cout << "enter optimal temperature: ";
        cin >> optimal;
        menu();
    }

    else if (choice == "2")
    {
        setState();
    }
    else if (choice == "3")
    {
        cout << "Enter Inside Temperature: ";
        cin >> inside;

        cout << "Enter Outside Temperature: ";
        cin >> outside;

        cout << "Enter Time of every loop: ";
        cin >> delta__t;

        // simulation
        simulation();
    }
    else if (choice == "4")
    {
        return;
    }
}

int main()
{
    menu();

    return 0;
}