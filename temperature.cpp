#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

// global variable
string state__1, state__2;
double optimal, inside, outside, delta__t;
void menu();

// class for PID and set final power 
class PID
{
private:
    const float ki = 1.5, kp = 400, kd = 20; //=> it depends to PID and heater or cooler

public:
    double calculate(double error, double prev_error, double integral, double dt)
    {
        double d = (error - prev_error) / dt;
        double output = (error * kp) + (integral * ki) + (d * kd); // PID formula

        if (output > 8000) //=> beacuase the max power is +-8000
        {
            output = 8000;
        }
        if (output < -8000)
        {
            output = -8000;
        }

        return output;
    }
};

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
    cout << time << "\t" << in << "\t" << opt << "\t" << "\t" << wall << "\t" << "\t" << window << "\t" << "\t" << ch << "\t" << "\t" << "\t" << airCondition << endl;

    dataTable << "Time: " << time << " | " << "T__in: " << in << " | " << "T optimal: " << opt << " | " << "DT_wall: " << wall << " | " << "DT_window: " << window << " | " << "DT_cooler,heater" << ch << " | " << "air conditioner: " << airCondition << endl;
}

// main function for simulation
void simulation()
{
    Room r;
    Window w;
    PID pid;

    string isStop;
    double time = 0, p, error, prev_error = optimal - inside, integral = 0;

    while (isStop != "exit")
    {
        error = optimal - inside; // => calculate error for PID
        integral += error * delta__t;

        p = pid.calculate(error, prev_error, integral, delta__t); //=> calculate power with PID system

        string airCondition = setAirCondition(optimal, inside); // => set air condition
        if (airCondition == "off")                              //=> if heater or cooler should be off
        {
            p = 0;
        }

        double DT_wall = r.D__T__wall(outside, inside, delta__t); //=> delta t of wall , window , and air conditioner
        double DT_window_1 = w.D__T__window(outside, inside, delta__t, state__1);
        double DT_window_2 = w.D__T__window(outside, inside, delta__t, state__2);
        double DT_ch = p / 20000 * delta__t;

        double result = DT_wall + DT_window_1 + DT_window_2 + DT_ch; //=> final calculate for temperature

        printData(time, inside, optimal, DT_wall, DT_window_1 + DT_window_2, DT_ch, airCondition); //=> print all data in terminal

        time += delta__t;
        inside += result;
        prev_error = error; //=> set  previous error

        cin >> isStop;
    }
    menu();
}

// body of program
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
    dataTable.close();

    return 0;
}