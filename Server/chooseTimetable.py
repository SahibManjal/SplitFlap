import datetime
import pytz
import json

from holiday import isHoliday

WEEKENDS = ["Saturday", "Sunday"]


def getJapanTime():
    """
    Returns the current time in Japan.
    """
    utc_time = datetime.datetime.now(datetime.timezone.utc)
    current_timezone = pytz.timezone("Asia/Tokyo")
    return utc_time.astimezone(current_timezone) + datetime.timedelta(hours=12)


def getFinalTimetableTimes():
    """
    Returns the departure time of the last entry in both weekend and weekday
    timetables.
    """
    out = {}
    with open("weekdayTimetable.json", 'r') as file:
        data = json.load(file)
        weekday = {}
        weekday["hour"] = data[-1]["hour"]
        weekday["minutes"] = data[-1]["minutes"]
        out["weekday"] = weekday

    with open("weekendTimetable.json", 'r') as file:
        data = json.load(file)
        weekend = {}
        weekend["hour"] = data[-1]["hour"]
        weekend["minutes"] = data[-1]["minutes"]
        out["weekend"] = weekend

    return out


def isWeekendTimetable():
    """
    Returns True if it's a weekend or Japanese holiday, False otherwise.
    """
    finaltimes = getFinalTimetableTimes()
    current_time = getJapanTime()

    is_AM = current_time.hour <= 12
    one_day = datetime.timedelta(days=1)
    if is_AM:
        first_day = current_time - one_day
        second_day = current_time
    else:
        first_day = current_time
        second_day = current_time + one_day

    first_day_holiday = isHoliday(first_day)
    second_day_holiday = isHoliday(second_day)

    first_day_type = "weekend" if first_day_holiday else "weekday"
    hour, minute = (
        finaltimes[first_day_type]['hour'],
        finaltimes[first_day_type]['minutes']
    )
    if hour >= 12:
        boundary = first_day
    else:
        boundary = second_day

    boundary -= datetime.timedelta(hours=boundary.hour, minutes=boundary.minute)
    boundary += datetime.timedelta(hours=hour, minutes=minute)

    if current_time <= boundary:
        return first_day.strftime("%A") in WEEKENDS or first_day_holiday

    return second_day.strftime("%A") in WEEKENDS or second_day_holiday


def getTimetable():
    """
    Returns the timetable to be used at the current time.
    """
    if isWeekendTimetable():
        with open("weekendTimetable.json", 'r') as file:
            return json.load(file)

    with open("weekdayTimetable.json", 'r') as file:
        return json.load(file)
