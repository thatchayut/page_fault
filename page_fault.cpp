#include<iostream>
#include<ctime>
#include<cstdlib>
#include<string>
using namespace std;

//Find the minimum using from all elements in the frame
int minUsed(int arr_size, int arr[])
{
    int min = arr[0];
    int min_position;
    for(int i = 0; i < arr_size; i++)
    {
        if(arr[i] <= min)
        {
            min = arr[i];
            min_position = i;
        }
    }
    return min_position;
}

//Find the maximum waiting time from all elements in the frame 
int longestWait(int arr_size, int arr[])
{
    int max = arr[0];
    int max_position;
    for(int i = 0; i < arr_size; i++)
    {
        if(arr[i] >= max)
        {
            max = arr[i];
            max_position = i;
        }
    }
    return max_position;

}
//FIFO algorithm
void FIFO(int ref_size, int frame_size, int ref[], int frame[])
{
    int ref_FIFO[ref_size];
    int frame_FIFO[frame_size];
    int replace_position = 0;
    int page_fault_counter = 0;
    bool is_found;

    //Duplicate the reference string to be used in this function
    for(int i = 0; i < ref_size; i++)
    {
        ref_FIFO[i] = ref[i];
    }

    //Duplicate frames to be used in this function
    for(int i = 0; i < frame_size; i++)
    {
        frame_FIFO[i] = frame[i];
    }

    cout << "Reference string : ";
    for(int i = 0; i < ref_size; i++)
    {
        cout << ref_FIFO[i] <<" ";
    }
    cout << endl;

    cout << "FIFO process ... \n";
    for(int i = 0; i < ref_size; i++)
    {
        is_found = false;
        for(int j = 0; j < frame_size; j++)
        {
            //If it found the same number, exit this loop
            //and move to the next number in the reference string
            if(ref_FIFO[i] == frame_FIFO[j])
            {
                is_found = true;
                break;
            }
        }

        //If the number is not found in the frame, page fault occurs
        //Replace the element to a frame in FIFO manner
        if(is_found == false)
        {
            if(replace_position >= frame_size)
            {
                replace_position = 0;
            }
            frame_FIFO[replace_position] = ref_FIFO[i]; 
            page_fault_counter++;
            replace_position++;
        }

        //print current elements in frame
        for(int j = 0; j < frame_size; j++)
        {
            if(frame_FIFO[j] < 0)
            {
                cout << "x" << " ";
            }
            else
            {
                cout << frame_FIFO[j] << " ";
            }
        }
        cout << endl;
    }
    cout << "page fault count : " << page_fault_counter <<endl;
}
//End of FIFO algorithm

//Optimal algorithm
void Optimal(int ref_size, int frame_size, int ref[], int frame[])
{
    int page_fault_counter = 0;
    int replace_position = 0;
    int ref_optimal[ref_size];
    int frame_optimal[frame_size];
    int next_use[frame_size]; //array that keeps the distanct to the next used of each frame
    bool is_found;
    bool is_empty;
    
    //Duplicate the reference string to be used in this function
    for(int i = 0; i < ref_size; i++)
    {
        ref_optimal[i] = ref[i];
    }

    //Duplicate all frames to be used in this function
    for(int i = 0; i < frame_size; i++)
    {
        frame_optimal[i] = frame[i];
    }

    //Initialize value in an array
    for(int i = 0; i < frame_size; i++)
    {
        next_use[i] = -1;
    }

    cout << "Reference string : ";
    for(int i = 0; i < ref_size; i++)
    {
        cout << ref_optimal[i] <<" ";
    }
    cout << endl;

    cout << "Optimal algorithm process ...\n";
    //iterate each number in reference string
    for(int i = 0; i < ref_size; i++)
    {
        is_found = false;
        is_empty = false;

        //If the considered element is found in the frame, do nothing 
        for(int j = 0; j < frame_size; j++)
        {
            if(ref_optimal[i] == frame_optimal[j])
            {
                is_found = true;
                break;
            }
        }
        
        //If page fault occurs...
        if(is_found == false)
        {
            //Check if each position in an array is empty
            for(int j = 0; j < frame_size; j++)
            {
                if(frame_optimal[j] == -1)
                {
                    is_empty = true;
                    break;
                }
            }

            //Fill in an array if the current position is empty
            if(is_empty)
            {
                for(int j = 0; j < frame_size; j++)
                {
                    
                    if(frame_optimal[j] == -1)
                    {
                        frame_optimal[j] = ref_optimal[i];
                        break;
                    }
                }
            }
            else
            {
                //Iterate from the current number to the next use of this number in 
                //reference string
                for(int j = 0; j < frame_size; j++)
                {
                    for(int k = i; k < ref_size; k++)
                    {
                        //To check how far to the next use
                        if(frame_optimal[j] == ref_optimal[k])
                        {   
                            next_use[j] = k - i;
                            break;
                        }
                        else
                        {
                            //If element is not exist, assign the highest value to be a waiting time
                            next_use[j] = 10000000;
                        }
                    }
                    
                } 
                //Find the frame's element with maximum distance to the next use to be replaced
                replace_position = longestWait(frame_size, next_use);

                //Replace an element from reference string to the proper position
                frame_optimal[replace_position] = ref_optimal[i];
                next_use[replace_position] = -1;   
            }          
            page_fault_counter++;
        }
        //print current element in frame
        for(int j = 0; j < frame_size; j++)
        {
            if(frame_optimal[j] < 0)
            {
                cout << "x" << " ";
            }
            else
            {
                cout << frame_optimal[j] << " ";
            }
        }
        cout << endl;
    }
    cout << "page fault count : " << page_fault_counter <<endl;
}
//End of Optimal algorithm

//LRU algorithm
void LRU(int ref_size, int frame_size, int ref[], int frame[])
{
    int page_fault_counter = 0;
    int replace_position;
    int ref_LRU[ref_size];
    int frame_LRU[frame_size];
    int used[frame_size];
    // int used_counter = 0;
    bool is_found;
    bool is_empty;

    //Duplicate the reference string to be used in this function
    for(int i = 0; i < ref_size; i++)
    {
        ref_LRU[i] = ref[i];
    }

    //Duplicate all frames to be used in this function
    for(int i = 0; i < frame_size; i++)
    {
        frame_LRU[i] = frame[i];
    }

    //Initialize an array that is used to keep number of using of each element
    //from all frames
    for(int  i = 0; i < frame_size; i++)
    {
        // used[i] = -1;
        used[i] = 0;
    }

    cout << "Reference string : ";
    for(int i = 0; i < ref_size; i++)
    {
        cout << ref_LRU[i] <<" ";
    }
    cout << endl;

    cout << "LRU algorithm process ...\n";

    //Iterate through all of elements in reference string
    for(int i = 0; i < ref_size; i++)
    {
        is_found = false;
        is_empty = false;
        //Check if the current element is in th frame
        for(int j = 0; j < frame_size; j++)
        {
            if(ref_LRU[i] == frame_LRU[j])
            {
                is_found = true;
                // used_counter++;
                used[j]++;
                // used[j] = used_counter;
                break;
            }
        }


        //If page fault occurs ...
        if(is_found == false)
        {
            //Check if the current frame is empty
            for(int j = 0; j < frame_size; j++)
            {
                if(frame_LRU[j] == -1)
                {
                    is_empty = true;
                    break;
                }
            }

            //Fill the current value in a frame if a frame is empty
            if(is_empty)
            {
                for(int j = 0; j < frame_size; j++)
                {
                    if(frame_LRU[j] == -1)
                    {
                        frame_LRU[j] = ref_LRU[i];
                        break;
                    }
                }
            }
            else 
            {
                replace_position = minUsed(frame_size, used);
                frame_LRU[replace_position] = ref_LRU[i];
                used[replace_position] = 0;
            }
            page_fault_counter++;
        }

        //print current element in frame
        for(int j = 0; j < frame_size; j++)
        {
            if(frame_LRU[j] < 0)
            {
                cout << "x" << " ";
            }
            else
            {
                cout << frame_LRU[j] << " ";
            }
        }
        cout << endl;
    }
    cout << "page fault count : " << page_fault_counter << endl;
}
//End of LRU algorithm

int main()
{
    //declare variables
    int ref_size;
    int frame_size;
    int choice;
    int rand_value;
    int repeat_counter;
    int value = 0;
    int show_counter = 0;
    int FIFO_page_fault;
    bool check_choice = false;

    //Get all reqruired data
    cout << "Size of frame: ";
    cin >> frame_size; 
    cout << "Size of reference string : ";
    cin >> ref_size;
    do
    {
        cout << "Choose type of reference string => [1]Random [2]Repeat:  ";
        cin >> choice;
        if((choice == 1) || (choice == 2))
        {
            check_choice = true;
        }
    }while(check_choice == false);

    //initilize array for reference string
    int ref[ref_size];
    for(int i = 0; i < ref_size; i++)
    {
        ref[i] = -1; 
    }
    //initialize array for frame
    int frame[frame_size];
    for(int i = 0; i < frame_size; i++)
    {
        frame[i] = -1;
    }

    //Generate reference data
    if(choice == 1)
    {
        srand(time(NULL));
        for(int i = 0; i < ref_size; i++)
        {
            rand_value = rand()%ref_size;
            ref[i] = rand_value;
        }
    }
    else if(choice == 2)
    {
        cout << "Input number of element in repeated group: ";
        cin >> repeat_counter;
        for(int i =0 ; i < ref_size; i++)
        {
            if (value == repeat_counter)
            {
                value = 0;
            }
            ref[i] = value;
            value ++;
        }
    }

    //show reference string
    cout << endl;
    cout << "Input reference string : ";
    for(int i = 0; i < ref_size; i++)
    {
        cout << ref[i] <<" ";
        show_counter++;
    }
    cout << endl;

    //Test 3 algorithms
    FIFO(ref_size, frame_size, ref, frame);
    cout << endl;
    Optimal(ref_size, frame_size, ref, frame);
    cout<<endl;
    LRU(ref_size, frame_size, ref, frame);

    cout << "\nCount : " << show_counter <<endl;
    return 0;
}