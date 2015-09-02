<###############################################################################
#                     Aw, Craps: A dice-rolling simulator                      #
#                  Written by Scott Raiford for The Daily WTF                  #
#                              August 30th, 2015                               #
################################################################################

Run this on any Windows PowerShell version with: 
    PowerShell -ExecutionPolicy Bypass -File Craps.ps1 [-Verbose] [-Debug]
        [-WholeTurn | -Test] [[-Dice] <Sides> | [-Die] <Die>]

    -Verbose
        Displays detailed results of die rolls and engine information.

    -WholeTurn
        If this parameter is specified, the software will simulate an entire
        turn, noting "pass" and "don't pass" bets. By default, the simulation
        will only run the come-out phase.

    -Test
        Ignores the actual dice roll and instead runs several sequential tests
        of the come-out functionality, covering the range of dice outcomes.

    -Dice <Dice>
        A space-delimited list of dice sides. All parameters after the switches
        define the number of sides on a die to be used, though the parameter
        name can be specified if desired.

    <Die>
        An alias for the -Dice parameter. All unidentified values on the command
        line will be treated as die definitions. These names are optional.

Examples:
    
    PowerShell -ExecutionPolicy Bypass -File Craps.ps1 -WholeTurn
        This execution will run a normal turn of standard craps.

    PowerShell -ExecutionPolicy Bypass -File Craps.ps1 6 -6
        This execution will run the come-out phase of standard craps, but with
        a negative die. The rules and odds are the same as a normal game, but
        the numbers have been changed to protect the innocent.

    PowerShell -ExecutionPolicy Bypass -File Craps.ps1 -WholeTurn 20 20
        This execution will run a normal turn of craps using D20s, providing for
        the likelihood of much longer turns. This would appeal to nerds who may
        go to a casino rather than a darkened basement for an evening's fun...
        ...but that would never happen.

    PowerShell -ExecutionPolicy Bypass -File Craps.ps1 -WholeTurn 6 6 6
        This variation is rather evil. A full 25% of rolls will be either a 10
        or 11, which takes on the function of the 7 in a normal game. It results
        in typically shorter turns overall.
        
################################################################################
#                                                                              #
#   Explanations and WTFery are noted in the code comments. Here be dragons.   #
#                                                                              #
###############################################################################>

# First and foremost, this directive has little to do with cmdlets or binding,
# but for my purposes it enables debugging support. Write-Debug and
# Write-Verbose commands will be suppressed unless requested via parameters.
# It also enables a whole bunch of other functionality, but I don't know what it
# all does. Something about summoning a food-gift from the buzzing-sky-cart...
[CmdletBinding()]

# For the second bit of madness, PowerShell has its own parameter processing.
# It's quite feature-rich, but the syntax is wildly different from the rest of
# the language.
param (
    [Parameter(
        Mandatory=$false,
        ValueFromPipeline=$true,
        ValueFromRemainingArguments=$true,
        HelpMessage="Enter a list of dice sizes."
    )]
    [Alias("Die")]
    [int[]]$Dice = @(6,6),

    [Parameter(
        Mandatory=$False,
        HelpMessage="Ignores the actual dice roll and instead runs several sequential tests of the win/lose functionality, covering the range of dice outcomes."
    )]
    [switch]$Test = $false,

    [Parameter(
        Mandatory=$False,
        HelpMessage="Enable simulating the entire turn (with reports of when bets win or lose)"
    )]
    [switch]$WholeTurn = $false
)

Write-Verbose $("Dice to be rolled: " + ($Dice -join ", "))
Write-Verbose $("Simulation: " + $(if ($WholeTurn) { "Turn" } else {"Come-out"}) )

# After ninety lines of script, we begin the first productive section. Here we
# compute the range of values the dice may cover. There are assumptions here
# that the dice to be used have a minimum face value of 1 (of the same sign as
# the maximum value for that die), and that the possible outcomes for that die
# are equal to the set of counting numbers between the minimum and the maximum,
# inclusive, with equal probability for all outcomes.
# For example, a die that is defined as "6" can roll any of 1, 2, 3, 4, 5, or 6,
# with each outcome being equally likely. A die defined as "-6" can roll any of 
# -1, -2, -3, -4, -5, or -6, also with each being equally likely.
$Minimum = 0
$Maximum = 0
$Dice | ForEach-Object {
    if ($_ -lt [Math]::Sign($_)) {
        $Maximum += [Math]::Sign($_); $Minimum += $_
    } else {
        $Minimum += [Math]::Sign($_); $Maximum += $_
    }
}

# Lacking well-defined requirements for the rules of the game, I have brillantly
# devised a set of rules that results in a standard craps game when played with
# the standard pair of 6-sided dice.

# In short, the middle outcome (or two, if the range does not have a single
# midpoint) of all possible dice rolls is assumed to be the most common, and as
# such is the number to end a turn. This allows the odds of the game to be
# manipulated by carefully selecting the set of dice to use.
$Midpoint = ($Maximum + $Minimum) / 2
$Midpoint = @(
    if (($Midpoint - [Math]::floor($Midpoint)) -eq 0.5 ) {
        @(($Midpoint-.5),($Midpoint+.5))
    } else {
        [Math]::Round($Midpoint)
    }
)

# The basic bets are implemented here, as lists of numbers that will win or lose
# during the come-out phase. Outcomes that are not involved in come-out bets are
# assumed to be possible points, to end the come-out phase.
# Understanding Powershell's handling of lists is recommended as an introductory
# exercise for Programming 201. Providing therapy for those who understand is
# recommended as an exercise for Psychology 405.
$PassWins = @($Midpoint + ($Maximum - 1))                 # This + concatenates.
$PassLoses = @($Maximum, $Minimum, ($Minimum + 1))        # This + adds.
$PassPushes = @()
$DontPassWins = @($Minimum, ($Minimum + 1))               # This + adds.
$DontPassLoses = @($Midpoint + ($Maximum - 1))            # This + concatenates.
$DontPassPushes = @($Maximum)

[System.Collections.ArrayList]$PossiblePoints = @($Minimum .. $Maximum)
$PassWins + $PassLoses + $PassPushes + $DontPassWins + $DontPassLoses + $DontPassPushes | ForEach-Object { $PossiblePoints.Remove([int]$_) }
# And these pluses went wee wee wee all the way home.

# This is the most straightforward method I've found for creating clean tabular
# output in Powershell 1.0. Later versions make this method more compact, or
# provide the ability to use an ordered key-value list. Considering my prior
# experience with the Lucky Deuce, I can assume that my execution environment
# will be hostile.
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Minimum" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$Minimum" -PassThru | Write-Output
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Midpoint" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$Midpoint" -PassThru | Write-Output
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Maximum" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$Maximum" -PassThru | Write-Output
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Pass wins" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$PassWins" -PassThru | Write-Output
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Pass loses" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$PassLoses" -PassThru | Write-Output
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Pass pushes" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$PassPushes" -PassThru | Write-Output
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Don't Pass wins" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$DontPassWins" -PassThru | Write-Output
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Don't Pass loses" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$DontPassLoses" -PassThru | Write-Output
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Don't Pass pushes" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$DontPassPushes" -PassThru | Write-Output
New-Object PSObject | Add-Member -MemberType NoteProperty -Name Property -Value "Possible points" -PassThru | Add-Member -MemberType NoteProperty -Name Roll -Value "$PossiblePoints" -PassThru | Write-Output

# Here we will simulate the game.
$TestNumber = 0
$Point = $null
Write-Output "The game begins!"
do {
    # May Nuffle have mercy on our rolls.
    $Roll = 0
    # The total ($Roll) is what happens when you take the set of dice...
    $Rolls = $Dice | ForEach-Object { ` # ...roll them individually...
        $RollMin = $( if ($_ -lt 0) { $_ } else { 1 } )
        $RollMax = $( if ($_ -gt 0) { $_ } else { -1 } ) + 1
        $DieRoll = $( Get-Random -Minimum $RollMin -Maximum $RollMax )
        Write-Verbose $("Die was $DieRoll from $RollMin to " + ($RollMax - 1));
        # ...Look at each outcome...
        $DieRoll
    } | ForEach-Object {
        # ...and sum them.
        $Roll += $_;
        # We then gather the dice up...
        $_
    }
    # ...and assign the output list to $Rolls. In one single (multi-line)
    # statement, we have built a random list of rolled dice and computed their
    # sum. Isn't PowerShell powerful? It can shoot your foot right off!

    if ($Test) {
        # If the test mode is enabled, we will go to all of the aforementioned
        # effort to roll the dice, compute totals, and properly handle the
        # results, then discard it all and count.
        $Roll = @($Minimum .. $Maximum)[$TestNumber++]
    }

    Write-Output $("Roll is (" + ($Rolls -join ', ') + "), totaling $Roll")

    # We will process the come-out bets if we're in the come-out phase (either
    # because we're in test mode, or because there's no point, though @sadserver
    # would be quick to note that there is never any point.
    if ($Test -or $Point -eq $null) {
        if ($PassLoses.Contains($Roll)) { Write-Output "Pass loses" }
        if ($PassPushes.Contains($Roll)) { Write-Output "Pass pushes" }
        if ($PassWins.Contains($Roll)) { Write-Output "Pass wins" }
        if ($DontPassLoses.Contains($Roll)) { Write-Output "Don't Pass loses" }
        if ($DontPassPushes.Contains($Roll)) { Write-Output "Don't Pass pushes" }
        if ($DontPassWins.Contains($Roll)) { Write-Output "Don't Pass wins" }
        if ($PossiblePoints.Contains($Roll)) {
            Write-Output "Come-Out is ended. The point is $Roll"
            $Point = $Roll
        }
    } else {
        # If we aren't in the come-out phase, we must be in regular play.

        # A simple check to see if the player has made his point. A pointy-
        # haired manager would note that a slideshow and handouts might help 
        # him with that.
        if ($Point -eq $Roll) {
            Write-Output "Pass wins" "Don't pass loses" "The shooter plays again."
            break;
        }

        # $Midpoint is a list, but it may have only a single element. "-eq"
        # would be an elegant solution, but the midpoint might also be 0,
        # which is evaluated as false, so -eq won't give a useful result.
        # The remaining option is an ugly check loop. For reasons I can only
        # describe as "PowerShell", Contains() doesn't seem to work with
        # $Midpoint.
        $MidpointWasRolled = $False
        $Midpoint | ForEach-Object {
            Write-Verbose "$Roll compares to $_" 
            if ($Roll.ToString() -eq $_.ToString()) { $MidpointWasRolled = $True }
        }
        if ($MidpointWasRolled) {
            Write-Output "Pass loses" "Don't Pass wins" "The dice pass to the next player."
            break;
        }
    }
# The logic for when the player gets to roll again is a bit complicated in this
# simulation, so I broke the test into several lines to manage it. PowerShell,
# however, cannot handle line breaks between some (but not all) of its tokens.
# The single backtick tells the parser to ignore the next character, allowing me
# to embed line breaks without interfering with the farce of a parser.
} while                                                                        `
    (                                                                          `
        (                                                                      `
            (                                                                  `
                (-not $Test)                                                   `
            -and                                                               `
                $Point -eq $null                                               `
            )                                                                  `
        -or                                                                    `
            $WholeTurn                                                         `
        )                                                                      `
    -or                                                                        `
        (                                                                      `
            $Test                                                              `
        -and                                                                   `
            $TestNumber -lt ($Maximum - $Minimum + 1)                          `
        )                                                                      `
    )

<##############################################################################>