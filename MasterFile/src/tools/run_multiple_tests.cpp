#include "vpxt_test_declarations.h"
#include "vpxt_test_list.h"

int run_multiple_tests(int argc, char *argv[], string WorkingDir,  int NumberofTests)
{
    if (argc < 5)
    {
        if (argc < 4)
        {
            printf(
                "  run_multipule_tests \n\n"
                "    <Test Type>\n"
                "      <1 - Create Files and Run Tests>\n"
                "        <Input Text File>\n"
                "      <2 - Create Files only>\n"
                "        <Input Text File>\n"
                "      <3 - Run Tests For Existing Files>\n"
                "        <Input Test Directory>\n"
                "      <4 - Resume Test>\n"
                "        <Input Test Directory>\n"
                "\n");

            return 0;
        }
    }

    if (WorkingDir.length() > 175)
    {
        tprintf("\nError: Folder Path Length too long.\n");
        return 0;
    }

    char buffer[512] = "";
    char buffer2[512] = "";
    char identicalFileBuffer[3] = "";

    //int PassFail[9999] = {0};
    int *PassFail = new int[NumberofTests+2];

    string SummCompAndTest = "";
    string SummComp = "";
    string SummTest = "";

    char *input = argv[3];

    string StringAr[20];
    string TimeStampAr2[2];
    string SelectorAr[999];
    string SelectorAr2[999];

    unsigned long RunTimeRecAr[999];
    /////////////////////////////Record Time it took to run test/////////////////////////////
    int RecordRunTimes = 0; //If set to one will record run times of tests
    unsigned int RunTime1 = 0;
    unsigned int RunTime2 = 0;

    /////////////////////////////////////////////////////////////////////////////////////////

    int y = 0;
    int Buf1Var = 0;
    int TestsRun = 0;
    int PrintMe = 1;
    int DummyArgvVar = 1;
    int trackthis1 = 0;
    int trackthis2 = 0;
    int CommentBool = 0;
    int PassFailInt = 0;
    int SelectorArInt = 0;
    int TestsRunCatch = 0;
    int MakeNewTestrun = 1;
    int identicalFileVar = 1;
    int NumberOfTestsRun = 1;
    int TestType = atoi(argv[2]);
    int linesskippedtracker = 0;

    long FilePositionTracker = 0;
    long FilePositionTracker2 = 0;

    string TimeStampPrevious = "";

    string WorkDirFileStr = "";
    string WorkingTextFilestr = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "Summary";


    ////////////////////Sets Stage for Resume Mode//////////////////////
    if (TestType == 4)
    {
        // Read in whats been done already from the summary updating all variables needed
        // Start running the test from TestRuns.txt file since it is auto commented it will resume with the last
        // Test not completed.  So long as all arrays are updated properly and variables are as well it should work
        // Seemlessly.

        WorkingDir = argv[3];                                           //location of main dir in as workingdir

        WorkingTextFilestr = WorkingDir;
        WorkingTextFilestr.append(slashCharStr());
        WorkingTextFilestr.append("TestsRun.txt");                      //TestsRun.txt original file ided

        //Determines which test is in the process of being run and sets the correct text file as input file
        SummCompAndTest = WorkingDir;
        SummCompAndTest.append(slashCharStr());
        SummCompAndTest.append("Mode1Results.txt");     // Mode 1
        //SummCompAndTest.append("Summary_CompressionsANDTests.txt");       // Mode 1
        SummComp = WorkingDir;
        SummComp.append(slashCharStr());
        SummComp.append("Mode2Results.txt");                // Mode 2
        //SummComp.append("Summary_CompressionsOnly.txt");              // Mode 2
        SummTest = WorkingDir;
        SummTest.append(slashCharStr());
        SummTest.append("Mode3Results.txt");                        // Mode 3
        //SummTest.append("Summary_TestsOnly.txt");                     // Mode 3

        int TestRunning = 0;

        fstream SummCompAndTestFile;
        SummCompAndTestFile.open(SummCompAndTest.c_str(), fstream::in);

        if (SummCompAndTestFile.good())
        {
            TestRunning = 1;
        }

        fstream SummTestFile;
        SummTestFile.open(SummTest.c_str());

        if (SummTestFile.good())
        {
            TestRunning = 3;
        }

        fstream SummCompFile;
        SummCompFile.open(SummComp.c_str());

        if (SummCompFile.good())
        {
            if (TestRunning != 3)
            {
                TestRunning = 2;
            }

        }

        SummCompAndTestFile.close();
        SummTestFile.close();
        SummCompFile.close();

        //exits with error if it cant find a summary file
        if (TestRunning == 0)
        {
            printf("\nError: No Summary File found\n");
            delete [] PassFail;
            return 0;
        }

        ///Sets Correct input file
        string PriorResultInput;

        if (TestRunning == 1)
        {
            PriorResultInput = SummCompAndTest;
        }

        if (TestRunning == 2)
        {
            PriorResultInput =  SummComp;
        }

        if (TestRunning == 3)
        {
            PriorResultInput = SummTest;
        }

        fstream PriorResultInputFile;
        PriorResultInputFile.open(PriorResultInput.c_str());

        int TestsrunCount = 0;
        int VarToInput = 0;
        int p = 0;

        //read in and throw away header
        while (p < 7)
        {

            PriorResultInputFile.getline(buffer, 1024);
            p++;
        }

        //read in data
        while (!PriorResultInputFile.eof())
        {
            PriorResultInputFile.getline(buffer, 1024);
            string bufferStr = buffer;
            int u = 0;

            //0 number not read yet|1 TestType not read yet|2 TestFolder not read yet|3 Status not read yet
            while (buffer[u] != '\0' && buffer[u] != '\r' && u <= bufferStr.length())
            {
                if (buffer[u] != ' ')
                {
                    if (VarToInput == 5)
                    {
                        VarToInput = 0;
                    }

                    if (VarToInput == 3)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (buffer[u] != '\0' && buffer[u] != '\r')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;

                        }

                        buffer[u] = '\0';
                        string PassFailStr = buffer3;

                        if (PassFailStr.compare("Passed") == 0)
                        {
                            PassFail[TestsrunCount] = 1;
                        }

                        if (PassFailStr.compare("Failed") == 0)
                        {
                            PassFail[TestsrunCount] = 0;
                        }

                        if (PassFailStr.compare("Indeterminate") == 0)
                        {
                            PassFail[TestsrunCount] = 2;
                        }

                        if (PassFailStr.compare("SeeComboRunLog") == 0)
                        {
                            PassFail[TestsrunCount] = 3;
                        }

                        if (PassFailStr.compare("SeePSNRLog") == 0)
                        {
                            PassFail[TestsrunCount] = 4;
                        }

                        if (PassFailStr.compare("RandomTestCompleted") == 0)
                        {
                            PassFail[TestsrunCount] = 5;
                        }

                        if (PassFailStr.compare("MinTestPassed") == 0)
                        {
                            PassFail[TestsrunCount] = 8;
                        }

                        if (PassFailStr.compare("CompressionMade") == 0)
                        {
                            PassFail[TestsrunCount] = 10;
                        }

                        if (PassFailStr.compare("ErrorFileMismatch") == 0)
                        {
                            PassFail[TestsrunCount] = 11;
                        }

                        if (PassFailStr.compare("TestNotSupported") == 0)
                        {
                            PassFail[TestsrunCount] = 12;
                        }

                        VarToInput = 4;

                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    if (VarToInput == 2)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (a < 24)
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;

                        }

                        SelectorAr2[TestsrunCount] = buffer3;
                        SelectorAr2[TestsrunCount].append("\"");

                        VarToInput = 3;
                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    if (VarToInput == 1)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (buffer[u] != ' ')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;

                        }

                        SelectorAr[TestsrunCount] = buffer3;
                        VarToInput = 2;
                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    if (VarToInput == 0)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (buffer[u] != ' ')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;
                        }

                        VarToInput = 1;
                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    //resets var at end
                    if (VarToInput == 4)
                    {
                        int a = 0;

                        while (buffer[u] != '\0' && buffer[u] != '\r')
                        {
                            u++;
                            a++;
                        }

                        VarToInput = 0;
                    }
                }

                u++;
            }


            if (buffer[0] != '\0' && buffer[u] != '\r')
            {
                // if the first character of a line after the initial input is read happens to be a dash
                // it signifies the end of Tests being run and the start of the summary, meaning that all
                // operations have already been run and the tests should teminate.
                if (buffer[0] == '-')
                {
                    if (TestRunning == 1)
                    {
                        printf("\nAll tests and compressions have already been created and run.\n");
                        PriorResultInputFile.close();
                        exit(0);
                    }

                    if (TestRunning == 2)
                    {
                        cout << "\nAll compressions have already been created.\n";
                        PriorResultInputFile.close();
                        exit(0);
                    }

                    if (TestRunning == 3)
                    {
                        cout << "\nAll tests on compressions have already been run.\n";
                        PriorResultInputFile.close();
                        exit(0);
                    }
                }

                TestsrunCount++;
            }

        }

        TestType = TestRunning;
        SelectorArInt = TestsrunCount;
        NumberOfTestsRun = TestsrunCount;
        PassFailInt = TestsrunCount;
        TestsRun = TestsrunCount;
        TestsRunCatch = TestsrunCount;

        if (TestType == 3)
        {
            NumberOfTestsRun++;
        }

        PriorResultInputFile.close();

        WorkingDir.append("\"");

        PrintMe = 0;
        MakeNewTestrun = 0;
    }

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////Format Output///////////////////////////


    if (TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar();
        WorkingDir3[v+1] = '\0';

        WorkDirFileStr = WorkingDir3;
        //WorkDirFileStr.append(MyDir);
        //WorkDirFileStr.append("_CompressionsANDTests.txt");
        WorkDirFileStr.append("Mode1Results.txt");

    }

    if (TestType == 2)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar();
        WorkingDir3[v+1] = '\0';

        WorkDirFileStr = WorkingDir3;
        //WorkDirFileStr.append(MyDir);
        //WorkDirFileStr.append("_CompressionsOnly.txt");
        WorkDirFileStr.append("Mode2Results.txt");
    }

    if (TestType == 3)
    {
        WorkDirFileStr = argv[3];
        WorkDirFileStr.append(slashCharStr());
        //WorkDirFileStr.append(MyDir);
        //WorkDirFileStr.append("_TestsOnly.txt");
        WorkDirFileStr.append("Mode3Results.txt");
    }


    if (TestType != 4)
    {
        ////////////Formatting New Input TextFile///////////
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar();
        WorkingDir3[v+1] = '\0';

        WorkingTextFilestr = WorkingDir3;
        WorkingTextFilestr.append("TestsRun.txt");
        ///////////////////////////////////////////////////

    }

    ////////////////////////////////////////////////////////////////////
    /////////////////Makes New Working Text File From Input/////////////
    if (TestType != 3 && MakeNewTestrun == 1)
    {
        //This copies original driving text file over to a new formated one not used for Mode 4 or 3
        fstream infile;
        infile.open(input);

        if (!infile.good())
        {
            printf("\nInput File does not exist\n");
            delete [] PassFail;
            return 0;
        }

        string CreateDir2(WorkingDir);
        CreateDir2.insert(0, "md \"");
        CreateDir2.erase(CreateDir2.end() - 1);
        vpxt_make_dir_vpx(CreateDir2);

        ofstream outfile(WorkingTextFilestr.c_str());

        while (!infile.eof())
        {
            infile.getline(buffer, 512);

            if (buffer[0] == '%' || buffer[0] == '\0' || buffer[0] == '+' || buffer[0] == '\r')
            {
                outfile << buffer << "\n";
            }
            else
            {
                if (buffer[0] != ' ')
                {
                    outfile << " " << buffer << "\n";
                }
                else
                {
                    outfile << buffer << "\n";
                }
            }
        }

        infile.close();
        outfile.close();
    }

    if (TestType == 3)
    {


        WorkingDir = argv[3];                               //location of main dir in as workingdir

        WorkingTextFilestr = WorkingDir;
        WorkingTextFilestr.append(slashCharStr());
        WorkingTextFilestr.append("TestsRun.txt");          //TestsRun.txt original file ided

        WorkingDir.append("\"");

        //Need to go through and replace all '+' from input file with '-' so that test can keep track of "Tests Only" run
        fstream WorkingTextFilePrep;
        WorkingTextFilePrep.open(WorkingTextFilestr.c_str());

        while (!WorkingTextFilePrep.eof())
        {
            FilePositionTracker = WorkingTextFilePrep.tellg();
            WorkingTextFilePrep.getline(buffer, 1024);
            FilePositionTracker2 = WorkingTextFilePrep.tellg();

            string bufferstring = buffer;
            bufferstring.erase(0);

            if (buffer[0] == '+')
            {
                bufferstring.insert(0, " ");
                WorkingTextFilePrep.seekg(FilePositionTracker);
                WorkingTextFilePrep << bufferstring.c_str() ;
                WorkingTextFilePrep.seekg(FilePositionTracker2);
            }
        }


        WorkingTextFilePrep.close();

        string SummComp = argv[3];
        SummComp.append(slashCharStr());
        SummComp.append("Mode2Results.txt");
        //SummComp.append("Summary_CompressionsOnly.txt");

        fstream PriorResultInputFile2;
        PriorResultInputFile2.open(SummComp.c_str());

        if (!PriorResultInputFile2.good())
        {
            printf("\nError opening file: %s\n", SummComp.c_str());
            delete [] PassFail;
            return 0;
        }

        int TestsrunCount = 0;
        int VarToInput = 0;
        int p = 0;

        //read in and throw away header
        while (p < 7)
        {
            PriorResultInputFile2.getline(buffer, 1024);
            p++;
        }

        //read in data
        int runonce = 1;
        char buffer3[255];

        while (!PriorResultInputFile2.eof())
        {
            PriorResultInputFile2.getline(buffer, 1024);
            runonce = 1;
            int u = 0;
            //0 number not read yet|1 TestType not read yet|2 TestFolder not read yet|3 Status not read yet

            int Q = 35;

            while (Q < 35 + 24)
            {
                buffer3[u] = buffer[Q];
                Q++;
                u++;
            }

            SelectorAr2[TestsrunCount] = buffer3;
            SelectorAr2[TestsrunCount].append("\"");

            if (buffer[0] != '\0' && buffer[u] != '\r')
            {
                TestsrunCount++;
            }

            memset(buffer3, 0, sizeof(buffer3));
        }

        PriorResultInputFile2.close();
    }

    //////////////////////////////////////////////////////////////////////
    fstream WorkingTextFile;
    WorkingTextFile.open(WorkingTextFilestr.c_str());

    if (!WorkingTextFile.good())
    {
        printf("\nInput File does not exist\n");
        delete [] PassFail;
        return 0;
    }

    /////////////////////////////////////////Files are actualy processed and run here//////////////////////////////////////////////
    while (!WorkingTextFile.eof())
    {
        memset(buffer, 0, sizeof(buffer));

        const char *DummyArgv[999];
        DummyArgv[0] = argv[0];
        DummyArgvVar = 1;
        FilePositionTracker = WorkingTextFile.tellg();
        WorkingTextFile.getline(buffer, 1024);
        FilePositionTracker2 = WorkingTextFile.tellg();

        string bufferString = buffer;

        trackthis1++;

        //skips over any line starting with a % in the input file to allow for comenting
        if (buffer[0] == '%' || buffer[0] == '\0' || buffer[0] == '+' || buffer[0] == '-' || buffer[0] == '\r')
        {
            linesskippedtracker++;
        }
        else
        {
            Buf1Var = 0;

#if defined(_WIN32)

            while (buffer[Buf1Var] != '\0')
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\0')
                {
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\0')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar].clear();
                    StringAr[DummyArgvVar].insert(0, buffer2);
                    DummyArgvVar++;
                }
            }

#elif defined(linux)

            while (buffer[Buf1Var] != '\0')
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\0')
                {
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\0')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar] = buffer2;
                    DummyArgvVar++;
                }
            }

#elif defined(__APPLE__)

            while (buffer[Buf1Var] != '\r' && Buf1Var < bufferString.length())
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\r' && Buf1Var < bufferString.length())
                {
                    //cout << buffer[Buf1Var] << "\n";
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\r')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar] = buffer2;
                    DummyArgvVar++;
                }
            }

#elif defined(__POWERPC__)

            while (buffer[Buf1Var] != '\r' && Buf1Var < bufferString.length())
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\r' && Buf1Var < bufferString.length())
                {
                    //cout << buffer[Buf1Var] << "\n";
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\r')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar] = buffer2;
                    DummyArgvVar++;
                }
            }

#endif

            y = 1;

            while (y < DummyArgvVar)
            {
                DummyArgv[y] = StringAr[y].c_str();
                y++;
            }

            char NumberOfTestsRunChar[4];
            DummyArgv[y] = vpx_itoa_custom(NumberOfTestsRun, NumberOfTestsRunChar, 10);

            if (CommentBool == 0)
            {
                printf("\n");
                int selector = atoi(DummyArgv[1]);

                if (selector >= 0 && selector < 53)
                {
                    if (selector != 11 && selector != 33 && selector != 35 && selector != 0)
                        NumberOfTestsRun++;
                }

                TimeStampPrevious = TimeStampAr2[0];
                TimeStampAr2[0] = date_string();

                //set the correct tester type dir string input
                string TestDir = WorkingDir;

                if (TestType == 3)
                {
                    TestDir = argv[3];
                }

                if (selector == 0)
                {
                    if (TestType == 1)
                    {
                        //run_multiple_tests(DummyArgvVar, (char **)DummyArgv, WorkingDir,);
                        printf("Not Implemented Yet.\n");
                    }
                }

                if (selector == AlWDFNUM)
                {
                    SelectorAr[SelectorArInt] = "AllowDropFrames";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_allow_drop_frames(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == ALWLGNUM)
                {
                    SelectorAr[SelectorArInt] = "AllowLagTest";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_allow_lag(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == ALWSRNUM)
                {
                    SelectorAr[SelectorArInt] = "AllowSpatialResampling";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_allow_spatial_resampling(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == AUTKFNUM)
                {
                    SelectorAr[SelectorArInt] = "AutoKeyFramingWorks";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_auto_key_frame(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == BUFLVNUM)
                {
                    SelectorAr[SelectorArInt] = "BufferLevelWorks";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_buffer_level(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == CPUDENUM)
                {
                    SelectorAr[SelectorArInt] = "CPUDecOnlyWorks";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_change_cpu_dec(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == CHGWRNUM)
                {
                    SelectorAr[SelectorArInt] = "ChangeCPUWorks";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_change_cpu_enc(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == DFWMWNUM)
                {
                    SelectorAr[SelectorArInt] = "DropFramesWaterMarkWorks";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_drop_frame_watermark(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == DTARTNUM)
                {
                    SelectorAr[SelectorArInt] = "DataRateTest";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_data_rate(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == DBMRLNUM)
                {
                    SelectorAr[SelectorArInt] = "DebugMatchesRelease";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_debug_matches_release(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == ENCBONUM)
                {
                    SelectorAr[SelectorArInt] = "EncoderBreakOut";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_encoder_break_out(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == ERRMWNUM)
                {
                    SelectorAr[SelectorArInt] = "ErrorResilientModeWorks";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_error_resolution(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == EXTFINUM)
                {
                    SelectorAr[SelectorArInt] = "ExtraFileCheck";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_extra_file(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == FIXDQNUM)
                {
                    SelectorAr[SelectorArInt] = "FixedQ";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_fixed_quantizer(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == FKEFRNUM)
                {
                    SelectorAr[SelectorArInt] = "ForceKeyFrameWorks";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_force_key_frame(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == FRSZTNUM)
                {
                    SelectorAr[SelectorArInt] = "FrameSizeTest";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_frame_size(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == GQVBQNUM)
                {
                    SelectorAr[SelectorArInt] = "GoodQualityVsBestQuality";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_good_vs_best(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == LGIFRNUM)
                {
                    SelectorAr[SelectorArInt] = "LagInFramesTest";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_lag_in_frames(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MAXQUNUM)
                {
                    SelectorAr[SelectorArInt] = "MaxQuantizerTest";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_max_quantizer(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MEML1NUM)
                {
                    SelectorAr[SelectorArInt] = "MemLeakCheck";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_mem_leak(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MEML2NUM)
                {
                    SelectorAr[SelectorArInt] = "MemLeakCheck2";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_mem_leak2(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MINQUNUM)
                {
                    SelectorAr[SelectorArInt] = "MinQuantizerTest";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_min_quantizer(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == MULTTNUM)
                {
                    SelectorAr[SelectorArInt] = "MultiThreadedTest";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_multithreaded(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == NVOPSNUM)
                {
                    SelectorAr[SelectorArInt] = "NewVsOldPSNR";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_new_vs_old_psnr(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == NVORTNUM)
                {
                    SelectorAr[SelectorArInt] = "NewVsOldRealTimeSpeed";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_new_vs_old_real_time_speed(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == NOISENUM)
                {
                    SelectorAr[SelectorArInt] = "NoiseSensitivityWorks";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_noise_sensitivity(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == OV2PSNUM)
                {
                    SelectorAr[SelectorArInt] = "OnePassVsTwoPass";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_one_pass_vs_two_pass(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == PLYALNUM)
                {
                    SelectorAr[SelectorArInt] = "PlayAlternate";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_play_alternate(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == POSTPNUM)
                {
                    SelectorAr[SelectorArInt] = "PostProcessorWorks";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_post_processor(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == RECONBUF)
                {
                    SelectorAr[SelectorArInt] = "ReconBuffer";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_reconstruct_buffer(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == RSDWMNUM)
                {
                    SelectorAr[SelectorArInt] = "ResampleDownWaterMark";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_resample_down_watermark(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == SPEEDNUM)
                {
                    SelectorAr[SelectorArInt] = "SpeedTest";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_speed(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == TVECTNUM)
                {
                    SelectorAr[SelectorArInt] = "TestVectorCheck";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_vector_test(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == TV2BTNUM)
                {
                    SelectorAr[SelectorArInt] = "TwoPassVsTwoPassBest";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_two_pass_vs_two_pass_best(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == UNDSHNUM)
                {
                    SelectorAr[SelectorArInt] = "UnderShoot";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_undershoot(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == VERSINUM)
                {
                    SelectorAr[SelectorArInt] = "Version";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = version_test(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                if (selector == WMLMMNUM)
                {
                    SelectorAr[SelectorArInt] = "WinLinMacMatch";
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2, TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                    {
                        RunTime1 = vpxt_get_time();
                    }

                    PassFail[PassFailInt] = test_win_lin_mac_match(DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2, TestType);

                    if (RecordRunTimes == 1)
                    {
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] = vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;
                }

                ////////////////////////////////////On Fly out put/////////////////////
                FILE *fp;

                if ((fp = freopen(WorkDirFileStr.c_str(), "a", stderr)) == NULL)
                {
                    printf("Cannot open summary out put file.\n");
                    delete [] PassFail;
                    return 0;
                }

                if (PrintMe == 1)
                {

                    if (TestType == 1)
                    {
                        fprintf(stderr, "\n-------------------------------------------------------------------------------\n\n"
                                "                                  Test Results \n");
                    }

                    if (TestType == 2)
                    {
                        fprintf(stderr, "\n-------------------------------------------------------------------------------\n\n"
                                "                                Test Compressions \n");
                    }

                    if (TestType == 3)
                    {

                        fprintf(stderr, "\n-------------------------------------------------------------------------------\n\n"
                                "                                Test Only Results \n");
                    }

                    fprintf(stderr, "\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");

                    PrintMe = 0;
                }



                SelectorAr2[SelectorArInt].erase(SelectorAr2[SelectorArInt].end() - 1);
                fprintf(stderr, "%4i %-30s%-28s", SelectorArInt, SelectorAr[SelectorArInt].c_str(), SelectorAr2[SelectorArInt].c_str());

                SelectorAr2[SelectorArInt].append("\"");

                if (PassFail[SelectorArInt] == 1)
                {
                    fprintf(stderr, "Passed\n");
                }

                if (PassFail[SelectorArInt] == 0)
                {
                    fprintf(stderr, "Failed\n");
                }

                if (PassFail[SelectorArInt] == 2)
                {
                    fprintf(stderr, "Indeterminate\n");
                }

                if (PassFail[SelectorArInt] == 3)
                {
                    fprintf(stderr, "SeeComboRunLog\n");
                }

                if (PassFail[SelectorArInt] == 4)
                {
                    fprintf(stderr, "SeePSNRLog\n");
                }

                if (PassFail[SelectorArInt] == 5)
                {
                    fprintf(stderr, "RandomTestCompleted\n");
                }

                if (PassFail[SelectorArInt] == 8)
                {
                    fprintf(stderr, "MinTestPassed\n");
                }

                if (PassFail[SelectorArInt] == 10)
                {
                    fprintf(stderr, "CompressionMade\n");
                }

                if (PassFail[SelectorArInt] == 11)
                {
                    fprintf(stderr, "ErrorFileMismatch\n");
                }

                if (PassFail[SelectorArInt] == 12)
                {
                    fprintf(stderr, "TestNotSupported\n");
                }

                fclose(fp);



                WorkingTextFile.seekg(FilePositionTracker);
                string bufferstring = buffer;
                bufferstring.erase(0);

                if (TestType == 3)
                {
                    bufferstring.insert(0, "-");
                }
                else
                {
                    bufferstring.insert(0, "+");
                }

                WorkingTextFile << bufferstring.c_str() ;

                WorkingTextFile.seekg(FilePositionTracker2);
                ////////////////////////////////////////////////////////////////////////////////


                PassFailInt++;
                SelectorArInt++;
            }

            ////////////////////////////////////////////////////////////////////////////////
            CommentBool = 0;
        }
    }

    WorkingTextFile.close();
    //outputs a summary of the test results

    if (TestsRun == 0)
    {
        cout << "Test file specified contains no tests.\n";
        delete [] PassFail;
        return 0;
    }

    if (TestsRun == TestsRunCatch)
    {
        if (TestType == 1)
        {
            printf("\nAll tests and compressions have already been created and run.\n");
            delete [] PassFail;
            return 0;
        }

        if (TestType == 2)
        {
            printf("\nAll compressions have already been created.\n");
            delete [] PassFail;
            return 0;
        }

        if (TestType == 3)
        {
            printf("\nAll tests on compressions have already been run.\n");
            delete [] PassFail;
            return 0;
        }
    }

    FILE *fp;

    if ((fp = freopen(WorkDirFileStr.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open summary out put file.\n");
        delete [] PassFail;
        return 0;
    }

    printf("\n\n\n");
    print_header_info();

    if (TestType == 1)
    {
        tprintf("\n-------------------------------------------------------------------------------\n\n"
                "                                  Test Results \n");
    }

    if (TestType == 2)
    {
        tprintf("\n-------------------------------------------------------------------------------\n\n"
                "                                Test Compressions \n");
    }

    if (TestType == 3)
    {
        tprintf("\n-------------------------------------------------------------------------------\n\n"
                "                                Test Only Results \n");
    }

    y = 0;

    tprintf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");

    while (y < TestsRun)
    {
        SelectorAr2[y].erase(SelectorAr2[y].end() - 1);
        tprintf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());

        if (PassFail[y] == 1)
        {
            tprintf("Passed\n");
        }

        if (PassFail[y] == 0)
        {
            tprintf("Failed\n");
        }

        if (PassFail[y] == 2)
        {
            tprintf("Indeterminate\n");
        }

        if (PassFail[y] == 3)
        {
            tprintf("SeeComboRunLog\n");
        }

        if (PassFail[y] == 4)
        {
            tprintf("SeePSNRLog\n");
        }

        if (PassFail[y] == 5)
        {
            tprintf("RandomTestCompleted\n");
        }

        if (PassFail[y] == 8)
        {
            tprintf("MinTestPassed\n");
        }

        if (PassFail[y] == 10)
        {
            tprintf("CompressionMade\n");
        }

        if (PassFail[y] == 11)
        {
            tprintf("ErrorFileMismatch\n");
        }

        if (PassFail[y] == 12)
        {
            tprintf("TestNotSupported\n");
        }

        y++;
    }

    if (TestType == 1)
    {
        tprintf("\n-------------------------------------------------------------------------------\n\n"
                "                              Test Results - Passed \n");
    }

    if (TestType == 3)
    {
        tprintf("\n-------------------------------------------------------------------------------\n\n"
                "                              Test Results - Passed \n");
    }

    //tprintf("\n\n");

    int TestIndicator = 0;

    if (TestType == 3 || TestType == 1)
    {
        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 1)
            {
                if (TestIndicator == 0)
                {
                    tprintf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                }

                TestIndicator = 1;
                tprintf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
                tprintf("Passed\n");
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf("NONE\n\n");
        }

        if (TestType == 1)
        {
            tprintf("\n-------------------------------------------------------------------------------\n\n"
                    "                             Test Results - MinPassed \n");
        }

        if (TestType == 3)
        {
            tprintf("\n-------------------------------------------------------------------------------\n\n"
                    "                             Test Results - MinPassed \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 8)
            {
                if (TestIndicator == 0)
                {
                    tprintf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                }

                TestIndicator = 1;
                tprintf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
                tprintf("MinTestPassed\n");
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf("NONE\n\n");
        }

        if (TestType == 1)
        {
            tprintf("\n-------------------------------------------------------------------------------\n\n"
                    "                           Test Results - Indeterminate \n");
        }

        if (TestType == 3)
        {
            tprintf("\n-------------------------------------------------------------------------------\n\n"
                    "                           Test Results - Indeterminate \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 2)
            {
                if (TestIndicator == 0)
                {
                    tprintf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                }

                TestIndicator = 1;
                tprintf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
                tprintf("Indeterminate\n");
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf("NONE\n\n");
        }

        if (TestType == 1)
        {
            tprintf("\n-------------------------------------------------------------------------------\n\n"
                    "                              Test Results - Failed \n");
        }

        if (TestType == 3)
        {
            tprintf("\n-------------------------------------------------------------------------------\n\n"
                    "                              Test Results - Failed \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 0)
            {
                if (TestIndicator == 0)
                {
                    tprintf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                }

                TestIndicator = 1;
                tprintf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());
                tprintf("Failed\n");
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf("NONE\n\n");
        }

        if (TestType == 1)
        {
            tprintf("\n-------------------------------------------------------------------------------\n\n"
                    "                               Test Results - Other \n");
        }

        if (TestType == 3)
        {
            tprintf("\n-------------------------------------------------------------------------------\n\n"
                    "                               Test Results - Other \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] != 0 && PassFail[y] != 1 && PassFail[y] != 8 && PassFail[y] != 2)
            {
                if (TestIndicator == 0)
                {
                    tprintf("\n\n%4s %-30s%-28s%s\n\n", "#", "               Test Type" , "     Test Folder", "Status");
                }

                TestIndicator = 1;
                tprintf("%4i %-30s%-28s", y, SelectorAr[y].c_str(), SelectorAr2[y].c_str());

                if (PassFail[y] == 3)
                {
                    tprintf("SeeComboRunLog\n");
                }

                if (PassFail[y] == 4)
                {
                    tprintf("SeePSNRLog\n");
                }

                if (PassFail[y] == 5)
                {
                    tprintf("RandomTestCompleted\n");
                }

                if (PassFail[y] == 10)
                {
                    tprintf("CompressionMade.\n");
                }

                if (PassFail[y] == 11)
                {
                    tprintf("ErrorFileMismatch\n");
                }

                if (PassFail[y] == 12)
                {
                    tprintf("TestNotSupported\n");
                }

            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf("NONE\n\n");
        }

        /////////////////////////////Output Time it took to run test/////////////////////////////
        if (RecordRunTimes == 1)
        {
            string TimeoutputStr = WorkDirFileStr;
            TimeoutputStr.erase(TimeoutputStr.length() - 4, 4);
            TimeoutputStr.append("_Times.txt");

            FILE *outputTime;
            outputTime = fopen(TimeoutputStr.c_str() , "w");

            printf("\n-------------------------------------------------------------------------------\n\n"
                   "                       Time to Run Tests\n");
            fprintf(outputTime, "\n-------------------------------------------------------------------------------\n\n"
                    "                       Time to Run Tests\n");

            printf("\n\n%4s %-28s%s\n\n", "", " Test Name", "            Time");
            fprintf(outputTime, "\n\n%4s %-28s%s\n\n", "", " Test Name", "            Time");

            y = 0;

            while (y < TestsRun)
            {
                printf("   %s", SelectorAr[y].c_str());
                fprintf(outputTime, "   %s", SelectorAr[y].c_str());

                int z = SelectorAr[y].size();

                while (z <= 25)
                {
                    printf(" ");
                    fprintf(outputTime, " ");
                    z++;
                }

                printf("%20u ms\n", RunTimeRecAr[y]);
                fprintf(outputTime, "%20u ms\n", RunTimeRecAr[y]);
                y++;
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////
    }

    fclose(fp);
    delete [] PassFail;
    return 0;
}