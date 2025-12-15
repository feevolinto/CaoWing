PROGRAM CacaoSupplySystem

    // --- DATA STRUCTURE DEFINITION ---
    STRUCTURE Supplier
        name AS STRING
        city AS STRING
        contact AS STRING
    END STRUCTURE

    // --- MAIN MODULE ---
    FUNCTION Main()
        // 1. Variable Declarations
        DECLARE suppliers[100] AS ARRAY OF Supplier
        DECLARE supplier_count AS INTEGER = 0
        DECLARE daily_usage[7] AS ARRAY OF FLOATS
        DECLARE current_inventory, reorder_threshold AS FLOATS
        DECLARE total_consumption, average_consumption AS FLOATS
        DECLARE choice AS INTEGER
        DECLARE has_data AS BOOLEAN = FALSE

        // 2. Load Database from File
        CALL LoadSuppliers(suppliers, REF supplier_count)

        // 3. Main Menu Loop
        WHILE choice IS NOT 5 DO
            PRINT "========================================"
            PRINT "   CACAO SUPPLY MANAGEMENT SYSTEM"
            PRINT "========================================"
            PRINT "[1] Input Weekly Consumption"
            PRINT "[2] Forecast Supply & Reorder Point"
            PRINT "[3] Find Suppliers by City"
            PRINT "[4] Save Report to File"
            PRINT "[5] Exit"
            PRINT "Enter choice: "
            INPUT choice

            SWITCH choice:
                CASE 1:
                    CALL InputConsumption(daily_usage)
                    CALL AnalyzeConsumption(daily_usage, REF total_consumption, REF average_consumption)
                    SET has_data = TRUE
                    BREAK

                CASE 2:
                    IF has_data IS FALSE THEN
                        PRINT "Error: Please input consumption data (Option 1) first."
                    ELSE
                        PRINT "Enter current inventory (in kg): "
                        INPUT current_inventory
                        PRINT "Enter reorder threshold (in kg): "
                        INPUT reorder_threshold
                        CALL ForecastSupply(current_inventory, average_consumption, reorder_threshold)
                    END IF
                    BREAK

                CASE 3:
                    CALL ViewSuppliers(suppliers, supplier_count)
                    BREAK

                CASE 4:
                    IF has_data IS FALSE THEN
                        PRINT "Error: No data available to save."
                    ELSE
                        CALL SaveReport(total_consumption, average_consumption, current_inventory)
                    END IF
                    BREAK

                CASE 5:
                    PRINT "Exiting system. Goodbye."
                    BREAK

                DEFAULT:
                    PRINT "Invalid input. Please try again."
            END SWITCH
            PRINT "Press specific key to continue..."
            WAIT FOR INPUT
        END WHILE
    END FUNCTION

    // --- SUB-FUNCTIONS ---

    // Function to load supplier data from a text file into the structure array
    FUNCTION LoadSuppliers(suppliers[], REF count)
        OPEN file "suppliers.txt" FOR READING
        IF file exists THEN
            WHILE NOT EndOfFile DO
                READ file_line
                PARSE file_line INTO suppliers[count].name, suppliers[count].city, suppliers[count].contact
                INCREMENT count
            END WHILE
            CLOSE file
        ELSE
            PRINT "Database file not found. Starting with empty supplier list."
        END IF
    END FUNCTION

    // Function to input daily usage using a loop
    FUNCTION InputConsumption(daily_usage[])
        PRINT "--- Enter Cacao Usage for 7 Days ---"
        FOR i FROM 0 TO 6 DO
            PRINT "Enter usage for Day ", (i + 1), ": "
            INPUT daily_usage[i]
        END FOR
    END FUNCTION

    // Function to calculate totals and analyze patterns (Uses Pointers/Reference for results)
    FUNCTION AnalyzeConsumption(daily_usage[], REF total, REF average)
        SET sum = 0
        SET min_usage = daily_usage[0]
        SET max_usage = daily_usage[0]

        // Calculate Sum and find Min/Max
        FOR i FROM 0 TO 6 DO
            sum = sum + daily_usage[i]
            IF daily_usage[i] < min_usage THEN min_usage = daily_usage[i]
            IF daily_usage[i] > max_usage THEN max_usage = daily_usage[i]
        END FOR

        // Assign to Reference Variables
        SET total = sum
        SET average = sum / 7

        PRINT "Weekly Total: ", total, " kg"
        PRINT "Daily Average: ", average, " kg"

        // Analyze Stability
        IF (max_usage - min_usage) > (average * 0.5) THEN
            PRINT "Status: Consumption is FLUCTUATING widely."
        ELSE
            PRINT "Status: Consumption is STABLE."
        END IF
    END FUNCTION

    // Function to predict when stock runs out
    FUNCTION ForecastSupply(inventory, daily_avg, threshold)
        DECLARE days_to_empty AS FLOAT
        DECLARE days_to_reorder AS FLOAT

        SET days_to_empty = inventory / daily_avg
        SET days_to_reorder = (inventory - threshold) / daily_avg

        PRINT "Current inventory will last approximately: ", days_to_empty, " days."

        // Selection logic for warnings
        IF inventory <= threshold THEN
            PRINT "CRITICAL ALERT: Inventory is below safety threshold! Reorder IMMEDIATELY."
        ELSE IF days_to_reorder <= 3 THEN
            PRINT "WARNING: You will reach the reorder point in ", days_to_reorder, " days."
        ELSE
            PRINT "Inventory Status: Safe. Reorder recommended in ", days_to_reorder, " days."
        END IF
    END FUNCTION

    // Function to search the structure array for suppliers in a specific city
    FUNCTION ViewSuppliers(suppliers[], count)
        DECLARE target_city AS STRING
        DECLARE found AS BOOLEAN = FALSE

        PRINT "Enter City to search (e.g., Davao, Cebu): "
        INPUT target_city

        PRINT "--- Suppliers in ", target_city, " ---"
        FOR i FROM 0 TO count - 1 DO
            IF suppliers[i].city IS EQUAL TO target_city THEN
                PRINT "Supplier Name: ", suppliers[i].name
                PRINT "Contact Info: ", suppliers[i].contact
                PRINT "-------------------------"
                SET found = TRUE
            END IF
        END FOR

        IF found IS FALSE THEN
            PRINT "No suppliers found in this area."
        END IF
    END FUNCTION

    // Function to save the analysis to a file
    FUNCTION SaveReport(total, average, inventory)
        OPEN file "report.txt" FOR APPENDING
        WRITE "Weekly Total: ", total
        WRITE "Daily Average: ", average
        WRITE "Remaining Inventory: ", inventory
        CLOSE file
        PRINT "Report saved to report.txt successfully."
    END FUNCTION

END PROGRAM