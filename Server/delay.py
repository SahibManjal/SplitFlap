from bs4 import BeautifulSoup
from playwright.sync_api import sync_playwright


def get_soup(url):
    """
    Loads the page to populate Javascript data and returns a parser with html
    and JavaScript information
    """
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=True)
        page = browser.new_page()
        page.goto(url)
        page.wait_for_timeout(3000)
        html = page.content()
        browser.close()
    return BeautifulSoup(html, "html.parser")


def isDelay():
    """
    Returns a list of (train destination, delay) for each train visible on a
    single direction of a Kintetsu line.
    """

    soup = get_soup(
        "https://tid.kintetsu.co.jp/LocationWeb/trainlocationinfoweb01.html"
    )

    # Find all the train service cells
    # make down/up a parameter?
    cells = soup.find_all("div", class_="train-info-up")

    trains = []
    for cell in cells:
        # Check if it has train info inside
        if cell:
            # Get destination
            dest_div = cell.find("div", class_="train-destination-text")
            destination = dest_div.get_text(strip=True) if dest_div else ""

            # Get delay time (default to 0 if empty)
            delay_span = cell.find("span", class_="delay-time")
            delay_text = delay_span.get_text(strip=True) if delay_span else ""

            # Parse delay -  "+01" format, default to 0
            if delay_text.startswith("+"):
                delay_minutes = int(delay_text[1:])
            elif delay_text:
                delay_minutes = int(delay_text)
            else:
                delay_minutes = 0

            trains.append({"destination": destination, "delay_minutes": delay_minutes})

    for t in trains:
        print(f"Destination: {t['destination']}, Delay: {t['delay_minutes']}")


isDelay()
