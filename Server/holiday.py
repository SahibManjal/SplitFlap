import requests
from bs4 import BeautifulSoup
import datetime
import pytz

WEEKENDS = ["Saturday", "Sunday"]
HOLIDAY_SCHEDULE_TYPE = ["National Holiday", "Bank holiday"]


def createParser(url):
    """
    Returns a BeautifulSoup parser with a header.
    """
    response = requests.get(url, headers={"User-Agent": "Mozilla/5.0"})
    return BeautifulSoup(response.content, "html.parser")


def getHolidayScheduleDays(soup):
    """
    Returns a list of holidays that Kintetsu uses a weekend timetable on. Each
    entry in the list includes the date, the holiday type, which day of the week
    it is, and the holiday name.
    """
    holidays = []
    table = soup.find("table", {"id": "holidays-table"})
    for row in table.find("tbody").find_all("tr"):
        column = row.find_all(["th", "td"])
        if len(column) >= 4:
            date = column[0].get_text(strip=True)
            holiday_type = column[3].get_text(strip=True)
            weekday = column[1].get_text(strip=True)
            name = column[2].get_text(strip=True)

            if weekday not in WEEKENDS and holiday_type in HOLIDAY_SCHEDULE_TYPE:
                holidays.append([date, weekday, name, holiday_type])
    return holidays


def isHoliday(japan_time):
    """
    Returns False if it's a weekend or Japanese holiday, True otherwise.
    """
    japan_year = japan_time.strftime("%Y")
    japan_day = japan_time.strftime("%b %-d")

    soup = createParser("https://www.timeanddate.com/holidays/japan/" + japan_year)
    holidays = getHolidayScheduleDays(soup)

    for holiday in holidays:
        if holiday[0] == japan_day:
            return True

    return False
